#ifndef _BOT_HH
#define _BOT_HH

#include <string>
#include <vector>

#include "libircclient.h"
#include "libirc_rfcnumeric.h"

#include "utils.hh"

class Bot {
  public:
    /* Creation and destruction of Bot. */

    // Create creates a Bot if it does not already exist. If it does, return the existing bot. If
    // an error occurs, returns nullptr.
    static Bot* Create(const char *botname) {
      if (Bot::bot_inst == nullptr) {
        Bot::bot_inst = new Bot(botname);
        if (Bot::bot_inst->event_code) {
          Bot::Destroy();
        }
        return Bot::bot_inst;
      }
      return Bot::bot_inst;
    }

    // Get returns the Bot. Returns nullptr if it does not exist.
    static Bot* Get(void) {
      return Bot::bot_inst;
    }

    // Destroy destroys the Bot.
    static void Destroy(void) {
      if (Bot::bot_inst != nullptr) {
        delete Bot::bot_inst;
        Bot::bot_inst = nullptr;
      }
    }

    // Assign sets the active bot to the given derived class instance.
    static void Assign(Bot *b) {
      if (b) Bot::bot_inst = b;
      else utils::PrintError("Bot is NULL or nullptr.");
    }

    /* Bot commands
     *
     * Note that all commands are simply requests from the IRC client to the IRC server. A
     * successfull request does not mean the action has been successfully applied. In fact, one
     * should check for the corresponding codes received by OnNumeric from the server.
     *
     * For each command function, we also list their corresponding codes for handling with the
     * OnNumeric callback if applicable.
     *
     * For more information, see: http://www.ulduzsoft.com/libircclient/
     */

    // Connect connects the bot to a server, but does not start the bot per se.
    int Connect(std::string srv_name);

    // Disconnect disconnects this bot from the server, but does not destroy it.
    void Disconnect(void);

    // Start starts the bot's networking loop. Only after starting the bot that the callbacks are
    // called.
    int Start(void);

    // Join forces the bot to join a password protected channel. This function must be called after
    // Start has already been called, and inside a callback function.
    //  LIBIRC_RFC_ERR_BANNEDFROMCHAN
    //  LIBIRC_RFC_ERR_INVITEONLYCHAN
    //  LIBIRC_RFC_ERR_BADCHANNELKEY
    //  LIBIRC_RFC_ERR_CHANNELISFULL
    //  LIBIRC_RFC_ERR_BADCHANMASK
    //  LIBIRC_RFC_ERR_NOSUCHCHANNEL
    //  LIBIRC_RFC_ERR_TOOMANYCHANNELS
    int Join(std::string channel, std::string password);

    // Join forces the bot to join a channel. This function must be called after Start has already
    // been called, and inside a callback function.
    int Join(std::string channel);

    // Say sends a message to the given channel.
    // If succeeded, then either no event is triggered or:
    //   LIBIRC_RFC_RPL_AWAY.
    // Else, if failed, the following event codes may be generated:
    //   LIBIRC_RFC_ERR_NORECIPIENT
    //   LIBIRC_RFC_ERR_NOTEXTTOSEND
    //   LIBIRC_RFC_ERR_CANNOTSENDTOCHAN
    //   LIBIRC_RFC_ERR_NOTONCHANNEL
    //   LIBIRC_RFC_ERR_NOTOPLEVEL
    //   LIBIRC_RFC_ERR_WILDTOPLEVEL
    //   LIBIRC_RFC_ERR_TOOMANYTARGETS
    //   LIBIRC_RFC_ERR_NOSUCHNICK
    virtual int Say(std::string msg, std::string channel);

    // Whisper sends a private message to a user.
    // If succeeded, then either no event is triggered or:
    //   LIBIRC_RFC_RPL_AWAY.
    // Else, if failed, the following event codes may be generated:
    //   LIBIRC_RFC_ERR_NORECIPIENT
    //   LIBIRC_RFC_ERR_NOTEXTTOSEND
    //   LIBIRC_RFC_ERR_CANNOTSENDTOCHAN
    //   LIBIRC_RFC_ERR_NOTONCHANNEL
    //   LIBIRC_RFC_ERR_NOTOPLEVEL
    //   LIBIRC_RFC_ERR_WILDTOPLEVEL
    //   LIBIRC_RFC_ERR_TOOMANYTARGETS
    //   LIBIRC_RFC_ERR_NOSUCHNICK
    virtual int Whisper(std::string msg, std::string user);

    // Broadcast sends a message to all channels this bot has joined.
    virtual int Broadcast(std::string msg);

    // Part causes the bot to attempt to leave a channel.
    //   LIBIRC_RFC_ERR_NOSUCHCHANNEL
    //   LIBIRC_RFC_ERR_NOTONCHANNEL
    int Part(std::string channel);

    // Users queries the server for a list of users currently in the given channel. This is only
    // the request function. You must handle this in the callback function OnNumeric by listening
    // for the codes:
    //   LIBIRC_RFC_RPL_NAMREPLY
    //   LIBIRC_RFC_RPL_ENDOFNAMES
    int Users(std::string channel);

    /* Bot callbacks */

    // OnConnect is a callback function called when the bot connects to the server.
    virtual void OnConnect(void) {}

    // OnNumeric is called when a certain numeric event code is received from the server.
    virtual void OnNumeric(std::string origin, std::vector<std::string> &params) {}

    // OnNick is called when a NICK message is received, i.e. someone changed their nick.
    //   onick - original nickname
    //   nnick - new nickname
    virtual void OnNick(std::string onick, std::string nnick) {}

    // OnQuit is called when someone disconnects from a channel.
    //   user   - user who disconnected
    //   reason - reason for disconnection
    virtual void OnQuit(std::string user, std::string reason) {}

    // OnJoin is called when someone joins the channel.
    //   user    - user who joined
    //   channel - channel joined
    virtual void OnJoin(std::string user, std::string channel) {}

    // OnPart is called when someone leaves the channel.
    //   user    - user who left
    //   channel - channel name
    //   reason  - reason for leaving
    virtual void OnPart(std::string user, std::string channel, std::string reason) {}

    // OnMessage is called when someone sends a message to the channel.
    //   user    - user who messaged
    //   channel - channel name
    //   text    - message text
    virtual void OnMessage(std::string user, std::string channel, std::string text) {}

    // OnPrivate is called when someone sends a private message to you.
    //   user - user who messaged
    //   nick - your own nickname
    //   text - message text
    virtual void OnPrivate(std::string user, std::string nick, std::string text) {}

  protected:
    // Bot constructor with the bot's username as parameter.
    Bot(const char *username);
    Bot(void) = delete;

    // Bot destructor.
    virtual ~Bot(void);

    // Fields
    std::string name;
    std::string server;
    std::string event_str;

    std::vector<std::string> channels;

    unsigned int event_code;
    bool running;
  private:
    // Singleton
    static Bot* bot_inst;


    irc_callbacks_t callbacks;
    irc_session_s *session;

    static void event_connect(irc_session_s*, const char *, const char *, const char **, unsigned int);
    static void event_quit(irc_session_s*, const char *, const char *, const char **, unsigned int);
    static void event_join(irc_session_s*, const char *, const char *, const char **, unsigned int);
    static void event_nick(irc_session_s*, const char *, const char *, const char **, unsigned int);
    static void event_part(irc_session_s*, const char *, const char *, const char **, unsigned int);
    static void event_channel(irc_session_s*, const char *, const char *, const char **, unsigned int);
    static void event_privmsg(irc_session_s*, const char *, const char *, const char **, unsigned int);
    static void event_numeric(irc_session_s*, unsigned int, const char *, const char **, unsigned int);
};

#endif
