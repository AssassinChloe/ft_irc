/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmercier <vmercier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 12:38:49 by vmercier          #+#    #+#             */
/*   Updated: 2022/07/25 12:38:50 by vmercier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void Command::quit()
{
    int index;
    std::string message = this->client->getPrefixe() + "QUIT Quit: " + this->argLine + "\r\n";
    if (this->client->getChanList().size() > 0)
    {
        std::map<std::string, std::string> tmp = this->client->getChanList();
        for (std::map<std::string, std::string>::iterator it = tmp.begin(); it != tmp.end(); it++)
        {
            index = this->server->getChannelIndex((*it).first);
            if (index >= 0)
                this->server->getChannel(index).broadcast(message);
        }
    }
    this->server->deleteClient(this->client->getFd());
}