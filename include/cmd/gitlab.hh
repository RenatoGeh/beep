#ifndef _CMD_GITLAB_HH
#define _CMD_GITLAB_HH

#include <string>
#include <vector>

#include <curl/curl.h>

#include <boost/property_tree/ptree.hpp>

#include "updateable.hh"
#include "chronos.hh"
#include "cmd/command.hh"

namespace cmd {
  class GitLab : public Command, public Updateable {
    public:
      GitLab(std::string group_id);
      ~GitLab(void);

      std::string Do(const std::vector<std::string> &params);

      void Update(void *arg);

    private:
      std::string group_id;
      CURL *handle, *uhandle;
      int error;
      struct memchunk {
        char *memory;
        size_t size;
      } buffer, ubuffer;

      chronos::Time last_update;

      static size_t write_data(void *contents, size_t size, size_t nmemb, void *userp);
      static CURL* create_handle(int &err, struct GitLab::memchunk *buf);
      static boost::property_tree::ptree* curl_to_pt(const std::string &url, CURL* h,
          struct GitLab::memchunk *b);
      static std::string query_and_parse(const std::string &group_id, std::string type,
          const std::string &stamp, CURL *hdl, struct GitLab::memchunk *buf);

      std::string do_any(std::string t, const std::vector<std::string> &params);
  };
}

#endif
