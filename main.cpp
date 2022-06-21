/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassassi <cassassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 16:56:57 by cassassi          #+#    #+#             */
/*   Updated: 2022/06/21 16:56:58 by cassassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"


int g_stop;

void sighandler(int sig)
{ 
    if (sig == SIGINT)
        g_stop = 1; 
    if (sig == SIGPIPE)
        g_stop = 2;
}

int main(int argc, char **argv)
{
    if (argc != 3)
	{
		std::cout << "./ircserv <port> <password>" << std::endl;
		return 1;
	}
	g_stop = 0;
	signal(SIGINT, sighandler);
    signal(SIGPIPE, sighandler); // attention pour l'instant je ne sais pas du tout ce qu'il faut faire en cas de SIGPIPE
    Server server(argv[1], argv[2]);
	if (server.init() < 0)
		return (1);
	while (g_stop == 0)
    {
        server.run();
	}
    return (0);
}