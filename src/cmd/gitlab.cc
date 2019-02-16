#include <cstdio>

#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

#include <curl/curl.h>
#include <boost/property_tree/json_parser.hpp>

#include "bot.hh"
#include "utils.hh"
#include "cmd/gitlab.hh"

#define GITLAB_URL "https://gitlab.com/api/v4/groups/"
#define GITLAB_MAX_ENTRIES 9

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
    std::string url = utils::Sprintf(GITLAB_URL "%s/%s?sort=desc", group_id, type);
    for (auto it = opts.begin(); it != opts.end(); ++it)
      url += "&" + *it;
    return url;
  }

  bool insane(std::string s) {
    int equals = 0;
    int n = s.size();
    for (int i = 0; i < n; ++i) {
      if (s[i] == '=') {
        ++equals;
        if (i == 0 || i+1 >= n || equals > 1) {
          return true;
        }
      }
    }
    return equals == 0;
  }
}

namespace cmd {
  size_t GitLab::write_data(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct GitLab::memchunk *mem = (struct GitLab::memchunk*) userp;

    char *ptr = (char*) realloc(mem->memory, mem->size + realsize + 1);
    if (ptr == NULL) {
      utils::PrintError("Not enough memory (realloc returned NULL)");
      return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
  }

  CURL* GitLab::create_handle(int &err, struct GitLab::memchunk *buf) {
    if (err) return nullptr;

    CURL *handle = curl_easy_init();

    if (!handle) {
      utils::PrintError("Could not initialize libcurl.");
      err = 1;
      return nullptr;
    }

    curl_easy_setopt(handle, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*) buf);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, GitLab::write_data);

    return handle;
  }

#define RESET_MEMCHUNK(mem) (mem).memory = (char*) malloc(1); \
                            (mem).size = 0;

  GitLab::GitLab(std::string gid) : Command("gl", "!gl",
      "GitLab Request command.", "GitLab Request"), group_id(gid), handle(nullptr) {
    curl_global_init(CURL_GLOBAL_DEFAULT);

    handle = create_handle(error, &buffer);
    uhandle = create_handle(error, &ubuffer);
    if (error) {
      curl_global_cleanup();
      return;
    }

    RESET_MEMCHUNK(buffer);
    RESET_MEMCHUNK(ubuffer);
  }

  GitLab::~GitLab(void) {
    if (!error) {
      curl_easy_cleanup(handle);
      curl_easy_cleanup(uhandle);
      curl_global_cleanup();
      free(buffer.memory);
    }
  }

  std::string GitLab::Do(const std::vector<std::string> &params) {
    if (params.size() <= 3) return "";
    std::string command = params.at(3);
    std::vector<std::string> opts = std::vector<std::string>(params.begin()+4, params.end());
    if (opts.empty())
      opts.push_back("state=opened");
    else
      opts.erase(std::remove_if(opts.begin(), opts.end(), insane), opts.end());
    if (command == "issues")
      return do_any("issues", opts);
    else if (command == "mr")
      return do_any("merge_requests", opts);
    return "";
  }

  boost::property_tree::ptree* GitLab::curl_to_pt(const std::string &url,
                                                  CURL* h,
                                                  struct GitLab::memchunk *b) {
    curl_easy_setopt(h, CURLOPT_URL, url.c_str());
    CURLcode res;
    res = curl_easy_perform(h);
    if (res != CURLE_OK)
      utils::PrintError("Unable to GET GitLab request (curl_easy_perform failed).");
    return parse_json(b->memory);
  }

namespace {
  std::string iterate_over(boost::property_tree::ptree *pt,
                           std::function<std::string(boost::property_tree::ptree&)> f) {
    std::string quote;
    int n = 0;
    for (auto it = pt->begin(); it != pt->end() && n < GITLAB_MAX_ENTRIES; ++n) {
      auto c = it->second;
      quote += f(c);
      if (++it != pt->end() && n+1 < GITLAB_MAX_ENTRIES)
        quote += "\n";
    }
    return quote;
  }
}

  std::string GitLab::do_any(std::string t, const std::vector<std::string> &params) {
    std::string url = query(group_id, t, params);
    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    boost::property_tree::ptree *pt = GitLab::curl_to_pt(url, handle, &buffer);
    t.pop_back();
    std::string quote = iterate_over(pt, [&](boost::property_tree::ptree &c) -> std::string {
        return utils::Sprintf("%s: %s (%s)", utils::toupper(t),
          c.get<std::string>("title").c_str(),
          c.get<std::string>("web_url").c_str());
      }
    );
    delete pt;
    free(buffer.memory);
    RESET_MEMCHUNK(buffer);

    return quote;
  }

  std::string GitLab::query_and_parse(const std::string &group_id, std::string type,
      const std::string &stamp, CURL *hdl, struct GitLab::memchunk *buf) {
    std::string url = query(group_id, type, std::vector<std::string> {stamp});
    boost::property_tree::ptree *pt = GitLab::curl_to_pt(url, hdl, buf);
    type.pop_back();
    std::string quote = iterate_over(pt, [&](boost::property_tree::ptree &c) -> std::string {
        bool new_obj = c.get<std::string>("created_at") == c.get<std::string>("updated_at");
        return utils::Sprintf("%s: %s: %s (%s)",
            new_obj ? "NEW" : "UPDATED",
            utils::toupper(type),
            c.get<std::string>("title").c_str(),
            c.get<std::string>("web_url").c_str());
      }
    );
    delete pt;
    return quote;
  }

  void GitLab::Update(void *arg) {
    Bot *b = static_cast<Bot*>(arg);
    chronos::Time now;
    std::string stamp = "updated_after=" + last_update.Format("%FT%T");
    b->Broadcast(query_and_parse(group_id, "issues", stamp, uhandle, &ubuffer));
    free(ubuffer.memory);
    RESET_MEMCHUNK(ubuffer);
    b->Broadcast(query_and_parse(group_id, "merge_requests", stamp, uhandle, &ubuffer));
    free(ubuffer.memory);
    RESET_MEMCHUNK(ubuffer);
    last_update = now;
  }
}
