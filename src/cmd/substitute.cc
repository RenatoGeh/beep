#include <cstdio>

#include <string>
#include <vector>
#include <list>
#include <regex>

#include "instant_log.hh"

#include "cmd/substitute.hh"

namespace {
  int split_query(std::string q, std::string *pat, std::string *rep) {
    int i = 0;
    for (auto it = q.begin(); it != q.end(); ++it, ++i)
      if (*it == '/' && it != q.begin() && *(it-1) != '\\') {
        *pat = "\\b" + q.substr(0, i);
        *rep = q.substr(i+1);
        return 0;
      }
    return rep->empty() ? -1 : -2;
  }

  inline bool matches(std::string line, std::string pat) {
    return std::regex_match(line, std::regex(".*" + pat + ".*"));
  }

  inline std::string replace(std::string line, std::string pat, std::string rep) {
    return std::regex_replace(line, std::regex("\\b" + pat), rep);
  }
}

namespace cmd {
  std::string Substitute::Do(const std::vector<std::string> &params) {
    if (params.size() < 4)
      return "";
    std::string pat, rep;
    int rv = split_query(params.at(3), &pat, &rep);
    if (rv < 0)
      return "";
    std::list<std::string> l = log->Cache();
    for (auto it = std::next(l.rbegin()); it != l.rend(); ++it) {
      std::string line = *it;
      if (line.empty())
        continue;
      if (matches(line, pat)) {
        std::string s = replace(line, pat, rep);
        return "Correction: " + s;
      }
    }
    return "";
  }
}
