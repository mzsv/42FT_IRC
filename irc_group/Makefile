
NAME	=	ft_irc

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
	Executor.cpp Response.cpp is_valid_nickname.cpp

OBJ_DIR = ./obj/

OBJS = $(patsubst %.cpp, $(OBJ_DIR)%.o, $(SRCS))

all: $(NAME)

$(OBJS): $(OBJ_DIR)%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -I$(INCLUDES) -I$(SERVER) -I$(USER) -I$(LOGGER) -o $@ -fPIE

$(NAME): $(OBJ_DIR) $(OBJS)
	@echo $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -I$(INCLUDES) -I$(SERVER) -I$(USER) -I$(LOGGER) -o $(NAME) -fPIE

$(OBJ_DIR):
	mkdir $@

fclean : clean
	$(RM) $(NAME)
	$(RM) -R $(OBJ_DIR)
	$(RM) *.a

clean :
	$(RM) $(NAME)*

re : fclean
	$(MAKE)

.PHONY: all clean fclean re
