/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   names.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmercier <vmercier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 12:34:32 by vmercier          #+#    #+#             */
/*   Updated: 2022/07/25 12:34:33 by vmercier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Command.hpp"

void Command::names(int index, std::string chanName)
{
    std::string list = "";
    std::map<int, Client*>  client_list = server->getChannel(index).getClientMap();

    for (std::map<int, Client*>::iterator it = client_list.begin(); it != client_list.end(); it++)
    {
        if (searchIfMode(CHAN_USER_MODE, (*it).second->getChanMode(chanName)) == 1 ||
        searchIfMode(USER_MODE, (*it).second->getStatus()) == 1)
            list = list + "@" + (*it).second->getNickname() + " ";
        else
            list = list + (*it).second->getNickname() + " ";
    }
    std::string message = RPL_NAMREPLY(this->client->getPrefixe(), this->client->getNickname(), chanName, list);
    send_message(*this->client, message);
    message = RPL_ENDOFNAMES(this->client->getPrefixe(), this->client->getNickname(), chanName);
    send_message(*this->client, message);
}