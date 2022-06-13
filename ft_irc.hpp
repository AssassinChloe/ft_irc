/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmercier <vmercier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 11:25:57 by cassassi          #+#    #+#             */
/*   Updated: 2022/06/13 10:41:27 by vmercier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_H
#define FT_IRC_H

# define MYPORT 8080
# define BACKLOG 3
# define BUFFER_SIZE 1024

# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <poll.h>
# include "string.h"
# include <iostream>
# include <csignal>


#endif