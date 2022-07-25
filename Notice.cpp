/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmercier <vmercier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 12:34:49 by vmercier          #+#    #+#             */
/*   Updated: 2022/07/25 12:35:05 by vmercier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void    Command::Notice()
{
    
    int i = 0;
    int nb_param = parameters.size();
    std::string message;
	if (checkRegistration() != 0)
    {
        message = ERR_NOTREGISTERED(this->client->getPrefixe(), check_params(this->client->getNickname()));
        send_message(*this->client, message);
        return;
    }
    if (nb_param == 0) 
            return;

    std::vector<std::string> target = ftsplit(parameters[0], ",");
    int nbtarget = target.size();
    for (i=0; i<nbtarget; i++)
    {

        if (target[i][0] == '#' || target[i][0] == '&' || target[i][0] == '+' || target[i][0] ==  '!')
        {

            target[i] = lowercase(target[i]); 
            message = this->client->getPrefixe() + "PRIVMSG " +  target[i] + " :" + this->getArgLine() + " \r\n";
            int index = server->getChannelIndex(target[i]);

            if (index != -1) 
            {

                int condition = 1;
                int index = server->getChannelIndex(target[i]);
                std::string modeChan = server->getChannel(index).getMode();
                if (searchIfMode('n', modeChan) == 1 && (server->getChannel(index).isOnChannel(client->getNickname()) == 0))
                    condition = 0;

                if (condition)
                {
                    std::map<int, Client*>  client_list = server->getChannel(index).getClientMap();
                    for (std::map<int, Client*>::iterator it = client_list.begin(); it != client_list.end(); it++)
                    {
                        if (this->client != (*it).second)
                            {
                                int id = (*it).second->getFd();
                                send(id, message.c_str(), message.size(), 0);
                            }
                    }
                }
            }
        }
        else
        {
            for (std::map<int, Client>::iterator it = this->server->getClientList().begin(); it!= this->server->getClientList().end(); it++)
            {
                if ((*it).second.getNickname() == target[i])
                {
                    int id = (it)->second.getFd();
                    message = this->client->getPrefixe() + "PRIVMSG " + target[i] + " :" + this->getArgLine() + " \r\n";
                    send(id, message.c_str(), message.size(), 0);
                }
            }
        }     
    }
}