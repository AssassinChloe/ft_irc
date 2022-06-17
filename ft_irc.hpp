/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmercier <vmercier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 11:25:57 by cassassi          #+#    #+#             */
/*   Updated: 2022/06/17 16:45:31 by vmercier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_H
#define FT_IRC_H


# define BACKLOG 10
# define BUFFER_SIZE 1024
# define DEBUG 1

# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <poll.h>
# include <string.h>
# include <iostream>
# include <csignal>
# include <errno.h> 
# include <stdio.h> 
# include <vector>
# include <sstream>
# include <iterator>
# include "Server.hpp"

# define SERVER_NAME "The Best"
# define VERSION "0.0"
# define USER_MODE "what"
# define CHAN_MODE "plop"
# define RPL_WELCOME(nick) (": Welcome to the Internet Relay Network " + nick + "\r\n")
# define RPL_YOURHOST(servername , version) (": Your host is " + servername + ", running version " + version + "\r\n")
# define RPL_MYINFO(servername, version, usr_modes, chann_modes) (": " + servername + " " + version + " " + usr_modes + " " + chann_modes + "\r\n")
# define RPL_CREATED(date) (": This server was created " + date + "\r\n");

extern int g_stop;
std::vector<std::string> ftsplit(std::string str, std::string delimiter);
#endif