#include <cstdio>

#include <sstream>
#include <string>
#include <vector>

#include <curl/curl.h>
#include <boost/property_tree/json_parser.hpp>

#include "utils.hh"
#include "cmd/gitlab.hh"

#define GITLAB_URL "https://gitlab.com/api/v4/groups/"

namespace {
  boost::property_tree::ptree* parse_json(char *mem) {
    std::stringstream ss;
    ss << mem;
    boost::property_tree::ptree *pt = new boost::property_tree::ptree;
    try {
      boost::property_tree::read_json(ss, *pt);
    } catch (boost::property_tree::json_parser::json_parser_error &exc) {
      utils::PrintError("Unable to parse JSON.");
      delete pt;
      pt = nullptr;
    }
    return pt;
  }

  std::string query(std::string group_id, std::string type, const std::vector<std::string> &opts) {
    std::string url = utils::Sprintf(GITLAB_URL "%s/%s", group_id, type);
    if (!opts.empty()) {
      auto it = opts.begin();
      url += "?" + *it;
      for (++it; it != opts.end(); ++it)
        url += "&" + *it;
    }
    return url;
  }
}

namespace cmd {
  size_t GitLab::write_data(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct GitLab::memchunk *mem = (struct GitLab::memchunk*) userp;

    char *ptr = (char*) realloc(mem->memory, mem->size + realsize + 1);
    if (ptr == NULL) {
      printf("Not enough memory (realloc returned NULL)\n");
      return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
  }

  GitLab::GitLab(std::string gid) : Command("gl", "!gl",
      "GitLab Request command.", "GitLab Request"), group_id(gid), handle(nullptr) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    handle = curl_easy_init();

    if (!handle) {
      utils::PrintError("Could not initialize libcurl.");
      curl_global_cleanup();
      error = 1;
      return;
    }

    curl_easy_setopt(handle, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&buffer);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, GitLab::write_data);

    buffer.memory = (char*) malloc(1);
    buffer.size = 0;
  }

  GitLab::~GitLab(void) {
    if (!error) {
      curl_global_cleanup();
      free(buffer.memory);
    }
  }

  std::string GitLab::Do(const std::vector<std::string> &params) {
    if (params.size() <= 3) return "";
    std::string command = params.at(3);
    std::vector<std::string> opts = std::vector<std::string>(params.begin()+4, params.end());
    if (command == "issue")
      return do_issue(opts);
    return "";
  }

  std::string GitLab::do_issue(const std::vector<std::string> &params) {
    std::string url = query(group_id, "issues", params);
    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());

    CURLcode res;
    res = curl_easy_perform(handle);
    if (res != CURLE_OK)
      utils::PrintError("Unable to GET GitLab request (curl_easy_perform failed).");
    curl_easy_cleanup(handle);

    boost::property_tree::ptree *pt = parse_json(buffer.memory);
    std::string quote;
    int n = 0;
    for (auto it = pt->begin(); it != pt->end(); ++it, ++n) {
      auto c = it->second;
      quote += utils::Sprintf("%s\nAccess at %s\n=====\n", c.get<std::string>("title").c_str(),
          c.get<std::string>("web_url").c_str());
    }
    quote += utils::Sprintf("Issues: %d", n);

    delete pt;
    free(buffer.memory);
    buffer.memory = (char*) malloc(1);
    buffer.size = 0;

    return quote;
  }
}
