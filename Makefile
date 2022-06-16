SRCS		:=  main.cpp Server.cpp
HEAD		:=	ft_irc.hpp
OBJS		:= $(SRCS:.cpp=.o)
NAME		:= ircserv
CC			:= c++
CFLAGS		:= -Wall -Wextra -Werror -std=c++98
%.o:		%.cpp $(HEAD)
			$(CC) $(CFLAGS) -c $< -o $@

all:		$(NAME)

$(NAME):	$(OBJS)
			$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
			$(RM) $(OBJS)
		
fclean:		clean
			$(RM) $(NAME)

re:			fclean all

.PHONY:     all clean fclean re