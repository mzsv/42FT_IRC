
NAME	=	ft_irc
BOT_NAME = bot

CXX		=	c++
CXXFLAGS	=	-Wall -Wextra -Werror --std=c++98 -g #-fsanitize=address
RM		=	rm -rf

INCLUDES = ./includes
SERVER = ./src/server/
LOGGER = ./src/logger/
USER = ./src/
BOT = ./src/bot/

SRC = ./src

vpath %.cpp src/
vpath %.cpp src/server/
vpath %.cpp src/utils/
vpath %.cpp src/logger/
vpath %.cpp src/bot/

SRCS = main.cpp check_argv.cpp split2queue.cpp Server.cpp Logger.cpp User.cpp Channel.cpp Message.cpp \
	Executor.cpp Response.cpp is_valid_nickname.cpp SignalHandler.cpp tolower_str.cpp

BOT_SRCS = split2queue.cpp tolower_str.cpp Logger.cpp Message.cpp TicTacToe.cpp Bot.cpp bot_main.cpp

OBJ_DIR = ./obj/
BOT_OBJ_DIR = ./obj_bot/

OBJS = $(patsubst %.cpp, $(OBJ_DIR)%.o, $(SRCS))
BOT_OBJS = $(patsubst %.cpp, $(BOT_OBJ_DIR)%.o, $(BOT_SRCS))

all: $(NAME) $(BOT_NAME)

$(OBJS): $(OBJ_DIR)%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -I$(INCLUDES) -I$(SERVER) -I$(USER) -I$(LOGGER) -I$(BOT) -o $@ -fPIE

$(BOT_OBJS): $(BOT_OBJ_DIR)%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -I$(INCLUDES) -I$(SERVER) -I$(USER) -I$(LOGGER) -I$(BOT) -o $@ -fPIE

$(NAME): $(OBJ_DIR) $(OBJS)
	@echo $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -I$(INCLUDES) -I$(SERVER) -I$(USER) -I$(LOGGER) -I$(BOT) -o $(NAME) -fPIE

$(BOT_NAME): $(BOT_OBJ_DIR) $(BOT_OBJS)
	@echo $(BOT_OBJS)
	$(CXX) $(CXXFLAGS) $(BOT_OBJS) -I$(INCLUDES) -I$(SERVER) -I$(USER) -I$(LOGGER) -I$(BOT) -o $(BOT_NAME) -fPIE

$(OBJ_DIR):
	mkdir $@

$(BOT_OBJ_DIR):
	mkdir $@

fclean : clean
	$(RM) $(NAME)
	$(RM) $(BOT_NAME)
	$(RM) -R $(OBJ_DIR)
	$(RM) -R $(BOT_OBJ_DIR)
	$(RM) *.a

clean :
	$(RM) $(OBJ_DIR) $(OBJ_DIR)*.o
	$(RM) $(BOT_OBJ_DIR) $(BOT_OBJ_DIR)*.o

re : fclean
	$(MAKE)

.PHONY: all clean fclean re
