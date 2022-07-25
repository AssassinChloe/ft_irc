/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Restart.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmercier <vmercier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 12:39:24 by vmercier          #+#    #+#             */
/*   Updated: 2022/07/25 12:39:27 by vmercier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void    Command::Restart()
{
    std::string clientStat = this->client->getStatus();
    if (searchIfMode('o', clientStat) != 1) // si client n'est pas operateur
    {
        std::string message = this->client->getPrefixe() + " 481 " + this->client->getNickname() + " :Permission Denied- You're not an IRC operator\r\n";
        send_message(*this->client, message);
        return ;
    }
    
    this->server->cleanClose();
    
	if (this->server->init() < 0)
		return;
	while (g_stop == 0)
    {
        this->server->run();
	}
    return ;

}