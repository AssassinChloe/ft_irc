/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmercier <vmercier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 11:25:57 by cassassi          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2022/07/25 11:38:47 by cassassi         ###   ########.fr       */
=======
/*   Updated: 2022/07/25 12:36:50 by vmercier         ###   ########.fr       */
>>>>>>> 1b3f5012b758937bfc14c561321678117561ee23
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_H
#define FT_IRC_H

# define BACKLOG 10
# define BUFFER_SIZE 1024
# define MAXCHANNEL 10
# define DEBUG 1
# define TOPIC_MAX_LEN 307
# define USERLEN 18

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
# include <stdio.h> 
# include <vector>
# include <deque>
# include <map>
# include <sstream>
# include <iterator>
# include <fstream>
# include <cstring>
# include <ctime>
# include "Server.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# include "Command.hpp"
# include <sys/stat.h>
# include <stdio.h>
# include <fcntl.h>
# include <cstdlib>
# include <string>


# define CONFIG_FILE "config.txt"
# define USER_MODE "o"
# define CHAN_USER_MODE "Oo"
# define CHAN_MODE "nit"
# define UNREGISTERED "U"
# define GANESH_FRIEND "g"


extern int g_stop;

std::vector<std::string> ftsplit(std::string str, std::string delimiter);
std::string check_params(std::string param);
int check_if_channel(std::string name);
int searchIfMode(std::string tofind, std::string target);
int searchIfMode(char tofind, std::string target);
void sighandler(int sig);
std::string lowercase(std::string s);
            
#endif