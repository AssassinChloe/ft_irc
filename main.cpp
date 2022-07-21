/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmercier <vmercier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 16:56:57 by cassassi          #+#    #+#             */
/*   Updated: 2022/07/21 11:18:28 by vmercier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

int g_stop;

void sighandler(int sig)
{ 
    if (sig == SIGINT)
        g_stop = 1; 
    if (sig == SIGPIPE)
        std::cout << "signal SIGPIPE intercepte" << std::endl;
        // g_stop = 2;
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
    signal(SIGPIPE, sighandler); 
    Server server(argv[1], argv[2]);
	if (server.init() < 0)
		return (1);
	while (g_stop == 0)
    {
        server.run();
	}
    server.cleanClose();
    return (0);
}