#include <cstdio>

#include <string>
#include <vector>
#include <stdexcept>
#include <map>

#include "cmd/smiley.hh"

namespace cmd {
  const std::map<std::string, std::string> SMILEYS = {
    {"lenny", "\u0028\u0020\u0361\u00B0\u0020\u035C\u0296\u0020\u0361\u00B0\u0029"},
    {"shrug", "\u00AF\\_(\u30C4)_/\u00AF"},
    {"disapproval", "\u0CA0_\u0CA0"},
    {"lennyblob", "\u0F3C \u3064  \u0361\u00B0 \u035C\u0296 \u0361\u00B0 \u0F3D\u3064"},
  };

  std::string Smiley::Do(const std::vector<std::string> &params) {
    if (params.size() < 4)
      return "";
    std::string rval, req = params.at(3);
    try {
      rval = SMILEYS.at(req);
    } catch (const std::out_of_range &exc) {
      printf("Requested smiley [%s] not found.\n", req.c_str());
      return "";
    }
    return rval;
  }
}
