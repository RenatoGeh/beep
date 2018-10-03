#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "libircclient.h"
#include "libirc_rfcnumeric.h"

void event_connect(irc_session_s *session, const char *event, const char *origin,
    const char **params, unsigned int count) {
  puts("Connected");
  if (irc_cmd_join(session, "#ccsl-usp", 0)) {
    puts("Connecting to ccsl-usp failed.");
    puts(irc_strerror(irc_errno(session)));
  }

  if (irc_cmd_msg(session, "#ccsl-usp", "Testing"))
    puts("Error sending message");
}

void event_numeric(irc_session_s *session, unsigned int event, const char *origin,
    const char **params, unsigned int count) {
}

int main() {
  irc_callbacks_t callbacks;

  memset(&callbacks, 0, sizeof(callbacks));
  callbacks.event_connect = event_connect;
  callbacks.event_numeric = event_numeric;

  irc_session_t *session = irc_create_session(&callbacks);

  if (!session) puts("Something went wrong.");

  if (irc_connect(session, "irc.freenode.net", 6667, 0, "CCSLTestBot", "CCSLTestBotName", "CCSL Test Bot")) {
    puts("Connecting to freenode failed.");
    puts(irc_strerror(irc_errno(session)));
  }

  printf("Connected? %d\n", irc_is_connected(session));
  if (irc_run(session)) {
    puts("Could not start loop.");
    puts(irc_strerror(irc_errno(session)));
  }
  printf("Connected? %d\n", irc_is_connected(session));

  irc_destroy_session(session);

  return 0;
}
