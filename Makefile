SRCS		:=  main.cpp Server.cpp Command.cpp Client.cpp Message.cpp Utils.cpp Channel.cpp\
				Join.cpp registration.cpp PingPong.cpp welcome.cpp Mode.cpp Who.cpp Privmsg.cpp oper.cpp\
				Part.cpp quit.cpp Topic.cpp Notice.cpp Kick.cpp List.cpp Kill.cpp names.cpp Invite.cpp

HEAD		:=	ft_irc.hpp Command.hpp Client.hpp Message.hpp Channel.hpp

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