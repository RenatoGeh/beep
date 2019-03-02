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
    {"lennymob", "\u0028 \u0361\u00b0\u0028 \u0361\u00b0 \u035c\u0296\u0028 \u0361\u00b0 \u035c\u0296 \u0361\u00b0\u0029\u0296 \u0361\u00b0\u0029 \u0361\u00b0\u0029"},
    {"lennyangry", "\u0028\u0e07 \u0360\u00b0 \u035f\u0644\u035c \u0361\u00b0\u0029\u0e07"},
    {"cuteblob", "\u0f3c \u3064 \u25d5\u005f\u25d5 \u0f3d\u3064"},
    {"lennywall", "\u252c\u2534\u252c\u2534\u2524 \u035c\u0296 \u0361\u00b0\u0029 \u251c\u252c\u2534\u252c\u2534"},
    {"lennyglad", "\u0028 \u0361\u1d54 \u035c\u0296 \u0361\u1d54 \u0029"},
    {"happy", "\u0298\u203f\u0298"},
    {"facepalm", "\u0028\u003e\u10da\u0029"},
    {"wink", "\u0ca0\u203f\u21bc"},
    {"lennyflip", "\u0028\u30ce\u0361\u00b0 \u035c\u0296 \u0361\u00b0\u0029\u30ce\ufe35\u253b\u253b"},
    {"flip", "\u0028\u256f\u00b0\u25a1\u00b0\u0029\u256f\ufe35 \u253b\u2501\u253b"},
    {"angryflip", "\u0028\uff89\u0ca0\u0434\u0ca0\u0029\uff89\ufe35\u253b\u2501\u253b"},
    {"unflip", "\u252c\u2500\u252c\u30ce\u0028\u0ca0\u005f\u0ca0\u30ce\u0029"},
    {"lennyglass", "\u0028 \u0361\u00b0 \u035c\u0296 \u0361\u00b0\u0029\u003e\u2310\u25a0\u002d\u25a0 \u0028\u2310 \u0361\u25a0 \u035c\u0296 \u0361\u25a0\u0029"},
    {"lennyglass1", "\u0028 \u0361\u00b0 \u035c\u0296 \u0361\u00b0\u0029\u003e\u2310\u25a0\u002d\u25a0"},
    {"lennyglass2", "\u0028\u2310 \u0361\u25a0 \u035c\u0296 \u0361\u25a0\u0029"},
    {"cry", "\u0f3c \u0ca5\u0644\u035f\u0ca5 \u0f3d"},
    {"lennysad", "\u0028 \u0361\u00b0 \u0296\u032f \u0361\u00b0\u0029"},
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
