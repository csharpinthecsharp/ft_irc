NAME	= ircserv
CPP	= c++
CFLAGS	= -std=c++98 -g -Wall -Wextra -Werror

SRCS	= src/Utils/Parser.cpp \
		  src/main.cpp \
		  src/Server.cpp

OBJS	= $(SRCS:.cpp=.o)

GREEN = \033[0;32m
BOLD = \033[1m
RESET = \033[0m

all: $(NAME)
	@echo ""
	@echo "$(GREEN)$(BOLD) ==== ./ircserv <port> <password> ====$(RESET)"
	@echo ""

%.o: %.cpp
	$(CPP) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CPP) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)
	make clean

re: fclean all

.PHONY: all clean fclean re
