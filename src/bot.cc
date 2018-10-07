#include <cstdio>
#include <cstring>

#include <string>
#include <vector>

#include "libircclient.h"
#include "libirc_rfcnumeric.h"

#include "bot.hh"
#include "utils.hh"

Bot* Bot::bot_inst = nullptr;

Bot::Bot(const char *username) {
  name = std::string(username);

  memset(&callbacks, 0, sizeof(callbacks));

  callbacks.event_connect = event_connect;
  callbacks.event_quit = event_quit;
  callbacks.event_join = event_join;
  callbacks.event_part = event_part;
  callbacks.event_nick = event_nick;
  callbacks.event_channel = event_channel;
  callbacks.event_privmsg = event_privmsg;
  callbacks.event_numeric = event_numeric;

  session = irc_create_session(&callbacks);

  event_code = 0;
  running = false;

  if (!session) {
    utils::PrintError("Could not create session.");
    event_code = 1;
  }
}

Bot::~Bot(void) {
  irc_destroy_session(session);
}

int Bot::Connect(std::string srv_name) {
  const char *bot_name = name.c_str();
  server.assign(srv_name);
  if (irc_connect(session, srv_name.c_str(), 6667, 0, bot_name, bot_name, NULL))
    return utils::PrintfError(session, "Could not connect to server [%s].", srv_name.c_str());
  return 0;
}

void Bot::Disconnect(void) {
  irc_disconnect(session);
}

int Bot::Join(std::string channel) {
  return Join(channel, "");
}

int Bot::Join(std::string channel, std::string password) {
  const char *pw = NULL;
  if (password == "")
    pw = password.c_str();
  if (utils::Contains(channels, channel))
    return utils::PrintfError(session, "Already in channel [%s].", channel.c_str());
  if (irc_cmd_join(session, channel.c_str(), pw))
    return utils::PrintfError(session, "Could not join channel [%s].", channel.c_str());
  channels.push_back(channel);
  return 0;
}

int Bot::Start(void) {
  if (irc_run(session)) {
    return utils::PrintError("Could not start networking loop.", session);
  }
  running = true;
  return 0;
}

int Bot::Say(std::string msg, std::string channel) {
  if (msg == "") return -1;
  if (irc_cmd_msg(session, channel.c_str(), msg.c_str()))
    return utils::PrintfError(session, "Could not send message to channel [%s].", channel.c_str());
  return 0;
}

int Bot::Whisper(std::string msg, std::string user) {
  if (msg == "") return -1;
  if (irc_cmd_msg(session, user.c_str(), msg.c_str()))
    return utils::PrintfError(session, "Could not send private message to user [%s].", user.c_str());
  return 0;
}

int Bot::Broadcast(std::string msg) {
  if (msg == "") return -1;
  int s = 0;
  for (auto it = channels.begin(); it != channels.end(); ++it) {
    int r = Say(msg, *it);
    if (r) s = r;
  }
  return s;
}

int Bot::Part(std::string channel) {
  if (irc_cmd_part(session, channel.c_str()))
    return utils::PrintfError(session, "Could not leave channel [%s].", channel.c_str());
  return 0;
}

int Bot::Users(std::string channel) {
  if (irc_cmd_names(session, channel.c_str()))
    return utils::PrintfError(session, "Could not retrieve user list from channel [%s].",
        channel.c_str());
  return 0;
}

void Bot::event_connect(irc_session_s*, const char *e, const char *o, const char **p, unsigned int c) {
  printf("%s, %s\n", e, o);
  for (unsigned int i = 0; i < c; ++i)
    printf("  %s\n", p[i]);
  Bot *b = Bot::Get();
  b->event_str.assign(e);
  b->OnConnect();
}

void Bot::event_quit(irc_session_s*, const char *e, const char *o, const char **p, unsigned int) {
  Bot *b = Bot::Get();
  b->event_str.assign(e);
  b->OnQuit(std::string(o), std::string(p[0]));
}

void Bot::event_join(irc_session_s*, const char *e, const char *o, const char **p, unsigned int) {
  Bot *b = Bot::Get();
  b->event_str.assign(e);
  b->OnJoin(std::string(o), std::string(p[0]));
}

void Bot::event_nick(irc_session_s*, const char *e, const char *o, const char **p, unsigned int) {
  Bot *b = Bot::Get();
  b->event_str.assign(e);
  b->OnNick(std::string(o), std::string(p[0]));
}

void Bot::event_part(irc_session_s*, const char *e, const char *o, const char **p, unsigned int) {
  Bot *b = Bot::Get();
  b->event_str.assign(e);
  b->OnPart(std::string(o), std::string(p[0]), std::string(p[1]));
}

void Bot::event_channel(irc_session_s*, const char *e, const char *o, const char **p, unsigned int) {
  Bot *b = Bot::Get();
  b->event_str.assign(e);
  b->OnMessage(std::string(o), std::string(p[0]), std::string(p[1]));
}

void Bot::event_privmsg(irc_session_s*, const char *e, const char *o, const char **p, unsigned int) {
  Bot *b = Bot::Get();
  b->event_str.assign(e);
  b->OnPrivate(std::string(o), std::string(p[0]), std::string(p[1]));
}

void Bot::event_numeric(irc_session_s*, unsigned int e, const char *o, const char **p, unsigned int c) {
  Bot *b = Bot::Get();
  b->event_code = e;
  std::vector<std::string> params(c);
  for (unsigned int i = 0; i < c; ++i) params[i].assign(p[i]);
  b->OnNumeric(std::string(o), params);
}
