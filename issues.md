# ISSUES




# SUGGESTIONS


- 2 ways to add user: channel->add_user and server->join_channel : choose one, Channel!
- add NULL check for every user/channel pointer access
- template function to get info from user/target/channel
- reply function with custom message (to send join message reply e.g)
- response: leave empty param when no value
- rethink params vs pointers
- throw errors and try catch them in the main
- add limit to number of targets on join and other who allow multiple
- replace \r\n with CRLF macro
- add return int to reply, easier to exit functions
- find or get pointer to find channel and user -> check for it.end() or NULL instead of always checking contains_ch/nick?
- LIST: implement
- store pointers instead of strings > add function to check for null pointers where needed
- PRIVMSG to several targets
- use references instead of pointers

# SOLVED/TAKEAWAYS


- ^D on an empty line: server unresponsive after that (for that client)
	- SOL: run nc with -q 0 (nc -q 0 localhost 1234), that way it guarantees to send an EOF 0 seconds after an EOF on stdin
	- man nc /-q: "-q seconds after EOF on stdin, wait the specified number of seconds and then quit. If seconds is negative, wait forever (default).  Specifying a non-negative seconds implies -N."
- other container for list of channels in user, something more remove/insert friendly, such as list

# REMEMBER
- Response needs a user and channel not null
-pfd.events = POLLIN;
	pfd.events |= POLLRDHUP;
	pfd.events |= POLLERR;
	pfd.events |= POLLHUP;
	pfd.events |= POLLNVAL;
	pfd.events |= POLLOUT;
	POLLWRBAND
	POLLWRNORM
	POLLPRI
- Singleton:
	Server& Server::get_instance()
	{
		static Server instance;
		return instance;
	}
   - Static Local Variable: static Server instance; - Inside the method, a static local variable named instance of type Server is declared and initialized. Because it's static, this variable is initialized only once, the first time the get_instance method is called. On subsequent calls, the same instance of Server is returned. The static local variable has a lifetime that extends across the entire runtime of the program, but its scope is limited to the get_instance method, making it accessible only through this method. [copilot]
   
   
