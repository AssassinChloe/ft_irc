/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassassi <cassassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 11:25:57 by cassassi          #+#    #+#             */
/*   Updated: 2022/06/20 14:21:14 by cassassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_H
#define FT_IRC_H

# define BACKLOG 10
# define BUFFER_SIZE 1024

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
# include <deque>
# include <map>
# include <sstream>
# include <iterator>
# include "Server.hpp"
# include "Client.hpp"
# include "Command.hpp"

extern int stop;

# define SERVER_NAME "Ganesh"
# define VERSION "0.1"
# define USER_MODE " une lettre au pif"
# define CHAN_MODE " une info serieuse"
# define CREATION "aujourd'hui vers maintenant"


#endif