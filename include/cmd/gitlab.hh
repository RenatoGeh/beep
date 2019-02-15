#ifndef _CMD_GITLAB_HH
#define _CMD_GITLAB_HH

#include <string>
#include <vector>

#include <curl/curl.h>

#include "chronos.hh"
#include "cmd/command.hh"

namespace cmd {
  class GitLab : public Command {
    public:
      GitLab(std::string group_id);
      ~GitLab(void);

      std::string Do(const std::vector<std::string> &params);

    private:
      std::string group_id;
      CURL *handle;
      int error;
      struct memchunk {
        char *memory;
        size_t size;
      } buffer;

      chronos::Time last_update;

      static size_t write_data(void *contents, size_t size, size_t nmemb, void *userp);

      std::string do_any(std::string t, const std::vector<std::string> &params);
  };
}

#endif
