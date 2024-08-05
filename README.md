# ft_irc - IRC Server

## Table of Contents

1. [Overview](#overview)
2. [Objectives](#objectives)
3. [Features](#features)
4. [How to use](#how-to-use)
5. [References](#references)
6. [Contributors](#contributors)
5. [Acknowledgments](#acknowledgments)

## Overview
ft_42 IRC Server is a custom-built Internet Relay Chat server designed to provide a robust and feature-rich communication platform. It adheres to the IRC protocol, allowing clients to connect, join channels, send messages, and interact in real-time. Moreover, it requires knowledge of how UNIX sockets work and networking.

This project involves the development of an IRC server in C++98 with the following requirements:
- The server must handle multiple clients simultaneously without hanging.
- All I/O operations must be non-blocking, and forking is not allowed.
- Only one `poll()` (or equivalent) can be used for handling all operations (read, write, listen, etc.).
- The server must communicate with clients via TCP/IP (v4 or v6).
- The server should be compatible with an official IRC client, with basic IRC functionalities implemented.
- Operators and regular users are supported with specific operator commands like KICK, INVITE, TOPIC, and MODE.

## Objectives
- Develop a fully functional IRC server compliant with the IRC protocol.
- Enable real-time communication between multiple clients.
- Implement essential IRC features such as channels, private messages, and user modes.
- Create a bot that can join channels and play games with users.
- Ensure the server is robust, handling various edge cases and maintaining stability.

## Features
- **Basic IRC Functionality**: Supports standard IRC commands like JOIN, PART, PRIVMSG, NICK, USER, and QUIT. Server password for authentication. Includes all relevant numeric replies and commands.
- **Channel Management**: Create, join, and leave channels with support for channel modes: invite-only (i), topic mode (t), channel key (k), channel operator (o) and user limit (l).
- **Bot Integration**: A bot that can be invited to channels to play a game, Tic Tac Toe.
- **Logging**: Server logging to monitor activities and debug issues.
- **Signal Handling**: Graceful shutdown on receiving SIGINT.
- **Reference client**: HexChat
- **Modular Design**: Clean and maintainable code with a modular structure.
- **List of supported commands**: PASS, NICK, USER, PING, PONG, JOIN, PART, NAMES, KICK, INVITE, TOPIC, MODE, PRIVMSG, NOTICE, MOTD, LUSERS, WHO, WHOIS and QUIT.
- **Numeric Replies - Command Responses**: RPL_WELCOME (001), RPL_YOURHOST (002), RPL_CREATED (003), RPL_MYINFO (004), RPL_ISUPPORT (005), RPL_LUSERCLIENT (251), RPL_LUSEROP (252), RPL_LUSERUNKNOWN (253), RPL_LUSERCHANNELS (254), RPL_LUSERME (255), RPL_LOCALUSERS (265), RPL_GLOBALUSERS (266), RPL_WHOISUSER (311), RPL_WHOISSERVER (312), RPL_ENDOFWHO (315), RPL_WHOISIDLE (317), RPL_ENDOFWHOIS (318), RPL_CHANNELMODEIS (324), RPL_CREATIONTIME (329), RPL_NOTOPIC (331), RPL_TOPIC (332), RPL_TOPICWHOTIME (333), RPL_INVITING (341), RPL_WHOREPLY (352), RPL_NAMREPLY (353), RPL_ENDOFNAMES (366), RPL_MOTD (372), RPL_MOTDSTART (375), RPL_ENDOFMOTD (376), RPL_WHOISHOST (378), RPL_WHOISMODES (379)
- **Numeric Replies - Error Messages**: ERR_NOSUCHNICK (401), ERR_NOSUCHCHANNEL (403), ERR_CANNOTSENDTOCHAN (404), ERR_NOORIGIN (409), ERR_NORECIPIENT (411), ERR_NOTEXTTOSEND (412), ERR_UNKNOWNCOMMAND (421), ERR_NOMOTD (422), ERR_NONICKNAMEGIVEN (431), ERR_ERRONEUSNICKNAME (432), ERR_NICKNAMEINUSE (433), ERR_USERNOTINCHANNEL (441), ERR_NOTONCHANNEL (442), ERR_USERONCHANNEL (443), ERR_NOTREGISTERED (451), ERR_NEEDMOREPARAMS (461), ERR_ALREADYREGISTERED (462), ERR_PASSWDMISMATCH (464), ERR_KEYSET (467), ERR_CHANNELISFULL (471), ERR_UNKNOWNMODE (472), ERR_INVITEONLYCHAN (473), ERR_BADCHANNELKEY (475), ERR_BADCHANMASK (476), ERR_CHANOPRIVSNEEDED (482), ERR_UMODEUNKNOWNFLAG (501), ERR_INVALIDKEY (525), ERR_INVALIDMODEPARAM (696)

## How to use

### Installation


1. Clone the repository:
   ```sh
   git clone <repository-url>

2. Compile the server and bot using the provided Makefile:
   ```sh
   make

3. Run the server with port number (for example: 6667) and password (for example: 111):
   ```sh
   ./ircserv 6667 111

4. Run the bot in a different terminal with the same port number and password:
    ```sh
    ./bot <port> <password>

### Setting up HexChat

- To test with HexChat, add a network to the Network List with the following settings:
    - <hostname>/<port> (example: "localhost/6669")
    - Tick "Accept invalid SSL certificates"
    - Tick "Use global user information"
    - Untick all other options on the list
    - Set Login Method to "Server password (/PASS password)"
    - Set Password to the one used to run the server
    - Set Character set to "UTF-8"
    - Connect
    - Note: commands on HexChat are sent in the format "/<command>" (example: "/join #channel")

### Connection using nc from the terminal

1. Connect to server:
    ```sh
    nc -q 0 <hostname> <port>
2. Register to the IRC server:
    ```sh
    PASS <password>
    NICK <nickname>
    USER <username> 0 * :<realname>

### Using the bot

- On HexChat:
    1. Create/join a channel: "/join #channel"
    2. Invite the bot to the channel: "/invite bot"
    3. Follow the intructions provided by the bot to play a game of Tic Tac Toe

- With nc:
    ```sh
    JOIN #channel
    INVITE bot #channel

## References

- [Modern IRC Client Protocol](https://modern.ircdocs.horse/)
- [Internet Relay Chat Protocol](https://datatracker.ietf.org/doc/html/rfc1459)
- [Internet Relay Chat: Channel Management](https://datatracker.ietf.org/doc/html/rfc2811)
- [Internet Relay Chat: Client Protocol](https://datatracker.ietf.org/doc/html/rfc2812)
- [Internet Relay Chat: Server Protocol](https://datatracker.ietf.org/doc/html/rfc2813)
- [Internet Relay Chat: Architecture](https://datatracker.ietf.org/doc/html/rfc2810)

## Contributors

- [Andre Silva](https://github.com/mzsv/)
- [Alexandru Mitcul](https://github.com/AlexMitcul)

## Acknowledgements

- The 42 School for providing the project specifications and environment.
