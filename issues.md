# ISSUES

- broadcast in channel: KICK, JOIN, PART and MODE. also PRIVMSG to channel and QUIT. TOPIC set/changed/cleared

- 2 ways to add user: channel->add_user and server->join_channel : choose one, Channel!
- INVITES: to be implemented
- JOIN: bradcast JOIN message
- JOIN 0: implement
- JOIN multiple: /JOIN #channel-one,#channel-two ,hunter2 (https://docs.inspircd.org/4/commands/#example-usage_7)
- JOIN check: Clients joining the channel in the future will receive a RPL_TOPIC numeric (or lack thereof) accordingly. (horse)
- PRIVMSG: sending mssgs to channels im not in. it doesnt show tho, check
- implement WHO: sent by hexchat when there are channels
- Registration: RPL_YOURESERVICE as per IRC protocol?
- Makefile: compilation flags
- add NULL check for every user/channel pointer access
- fix: empty params, generate message with empty value
- handle leaks
- repeated USER command is setting duplicated username (but not nick)
- add USERLEN limit to ISUPPORT and implement it. silently truncates username (horse)
- handle QUIT (from a QUIT message; EOF; other reason for having BREAK flag active)
- SEGFAULT: Logger::Log(DEBUG, "User " + users_[i]->get_nickname() + " BREAK flag activated.
- MOTD: server should make sure each line has max 80 (detail...)
- UNIX times are not being formatted by hexchat
- test MODE
- TARGMAX: check which commands take several target (except JOIN and PART, by default to take several)
pfd.events = POLLIN;
	pfd.events |= POLLRDHUP;
	pfd.events |= POLLERR;
	pfd.events |= POLLHUP;
	pfd.events |= POLLNVAL;
	pfd.events |= POLLOUT;
	POLLWRBAND
	POLLWRNORM
	POLLPRI


# SUGGESTIONS


- reply function with custom message (to send join message reply e.g)
- rethink params vs pointers
- throw errors and try catch them in the main
- add limit to number of targets on join and other who allow multiple
- replace \r\n with CRLF macro
- add join_message, part_message, error message as replies

# SOLVED/TAKEAWAYS


- ^D on an empty line: server unresponsive after that (for that client)
	- SOL: run nc with -q 0 (nc -q 0 localhost 1234), that way it guarantees to send an EOF 0 seconds after an EOF on stdin
	- man nc /-q: "-q seconds after EOF on stdin, wait the specified number of seconds and then quit. If seconds is negative, wait forever (default).  Specifying a non-negative seconds implies -N."
- other container for list of channels in user, something more remove/insert friendly, such as list

# REMEMBER
- Response needs a user and channel not null
