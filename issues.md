# EVALUATION



# ISSUES

- review Makefile

# SUGGESTIONS


- review lowercase
- review responses relying on trailing param
- user.set_password() what for?
- should Response be so static?
- unused chanop flag
- executor instances
- map user-op status for channels
- separate util functions from the classes methods .cpp
- setup exceptions
- send_message: add final \r\n, instead of doing when building the message?
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
- setup socket at constructor server level
- map of users in server
- implement LIST
- QUIT: get trailing as reason, instead of second arg

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
   

NAME	=	ft_irc
BOT		=	bot	# Bot

CXX		=	c++
CXXFLAGS	=	-Wall -Wextra -Werror --std=c++98 -g #-fsanitize=address
RM		=	rm -rf

INCLUDES = ./includes
SERVER = ./src/server/
LOGGER = ./src/logger/
USER = ./src/
SRC = ./src

vpath %.cpp src/
vpath %.cpp src/server/
vpath %.cpp src/utils/
vpath %.cpp src/logger/

SRCS = main.cpp check_argv.cpp split2queue.cpp Server.cpp Logger.cpp User.cpp Channel.cpp Message.cpp \
	Executor.cpp Response.cpp is_valid_nickname.cpp SignalHandler.cpp tolower_str.cpp
BOT_SRCS = Bot.cpp bot_main.cpp # Bot

OBJ_DIR = ./obj/

OBJS = $(patsubst %.cpp, $(OBJ_DIR)%.o, $(SRCS))
BOT_OBJS = $(patsubst %.cpp, $(OBJ_DIR)%.o, $(BOT_SRCS)) # Bot


all: $(NAME) $(BOT) # Bot

$(OBJS): $(OBJ_DIR)%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -I$(INCLUDES) -I$(SERVER) -I$(USER) -I$(LOGGER) -o $@ -fPIE

$(NAME): $(OBJ_DIR) $(OBJS)
	@echo $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -I$(INCLUDES) -I$(SERVER) -I$(USER) -I$(LOGGER) -o $(NAME) -fPIE

$(BOT): $(OBJ_DIR) $(BOT_OBJS) # Bot
	$(CXX) $(CXXFLAGS) $(BOT_OBJS) -I$(INCLUDES) -I$(SERVER) -I$(USER) -I$(LOGGER) -o $(BOT) -fPIE

$(OBJ_DIR):
	mkdir $@

fclean : clean 
	$(RM) $(NAME)
	$(RM) $(BOT)
	$(RM) -R $(OBJ_DIR)
	$(RM) *.a

clean : # Bot
	$(RM) $(NAME)* $(BOT)*

re : fclean
	$(MAKE)

.PHONY: all clean fclean re

_______


NAME	=	ft_irc
BOT		=	bot	# Bot

CXX		=	c++
CXXFLAGS	=	-Wall -Wextra -Werror --std=c++98 -g #-fsanitize=address
RM		=	rm -rf

INCLUDES = ./includes
SERVER = ./src/server/
LOGGER = ./src/logger/
USER = ./src/
SRC = ./src

vpath %.cpp src/
vpath %.cpp src/server/
vpath %.cpp src/utils/
vpath %.cpp src/logger/

SRCS = main.cpp check_argv.cpp split2queue.cpp Server.cpp Logger.cpp User.cpp Channel.cpp Message.cpp \
	Executor.cpp Response.cpp is_valid_nickname.cpp SignalHandler.cpp tolower_str.cpp
BOT_SRCS = Message.cpp Logger.cpp Bot.cpp split2queue.cpp tolower_str.cpp bot_main.cpp # Bot

OBJ_DIR = ./obj/

OBJS = $(patsubst %.cpp, $(OBJ_DIR)%.o, $(SRCS))
BOT_OBJS = $(patsubst %.cpp, $(OBJ_DIR)%.o, $(BOT_SRCS)) # Bot

all: $(NAME) $(BOT)

$(OBJS): $(OBJ_DIR)%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -I$(INCLUDES) -I$(SERVER) -I$(USER) -I$(LOGGER) -o $@ -fPIE

$(BOT_OBJS): $(OBJ_DIR)%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -I$(INCLUDES) -I$(SERVER) -I$(USER) -I$(LOGGER) -o $@ -fPIE

$(NAME): $(OBJ_DIR) $(OBJS)
	@echo $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -I$(INCLUDES) -I$(SERVER) -I$(USER) -I$(LOGGER) -o $(NAME) -fPIE

$(BOT) : $(OBJ_DIR) $(BOT_OBJS) # Bot
	@echo $(BOT_OBJS)
	$(CXX) $(CXXFLAGS) $(BOT_OBJS) -I$(INCLUDES) -I$(SERVER) -I$(USER) -I$(LOGGER) -o $(BOT) -fPIE

$(OBJ_DIR):
	mkdir $@

fclean : clean
	$(RM) $(NAME)
	$(RM) $(BOT)
	$(RM) -R $(OBJ_DIR)
	$(RM) *.a

clean :
	$(RM) $(NAME)*
	$(RM) $(BOT)*

re : fclean
	$(MAKE)

.PHONY: all clean fclean re

