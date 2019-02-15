#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include <string>
#include <vector>

#include "utils.hh"
#include "cmd/command.hh"

#include "cmd/leave.hh"

void genkey(char key[_CMD_LEAVE_KEY_SIZE]) {
  srand(time(NULL));
  const static char voc[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  for (int i = 0; i < _CMD_LEAVE_KEY_SIZE-1; ++i)
    key[i] = voc[rand() % sizeof(voc)];
  key[_CMD_LEAVE_KEY_SIZE-1] = '\0';
}

namespace cmd {
  Leave::Leave(void) : Command("leave", "!leave", "This command forces the bot to disconnect.",
          "disconnects the bot"), trigger(false) {
    genkey(pw);
    printf("\n===============\nLeave passkey: %s\n===============\n\n", pw);
  }

  std::string Leave::Do(const std::vector<std::string> &params) {
    if (params.size() < 4)
      return "";
    if (strcmp(params.at(3).c_str(), pw))
      return "";
    trigger = true;
    return utils::Sprintf("User %s asked me to leave. :(", params[0]);
  }
}

