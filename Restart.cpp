/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Restart.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassassi <cassassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 11:56:28 by cassassi          #+#    #+#             */
/*   Updated: 2022/07/25 14:31:42 by cassassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void    Command::Restart()
{
    std::string clientStat = this->client->getStatus();
    if (searchIfMode('o', clientStat) != 1)
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