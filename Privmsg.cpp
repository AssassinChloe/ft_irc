/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassassi <cassassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 12:00:46 by cassassi          #+#    #+#             */
/*   Updated: 2022/07/25 14:32:00 by cassassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void    Command::Privmsg()
{
    
    int i = 0;
    int nb_param = this->parameters.size();
    std::string message;
	if (checkRegistration() != 0)
    {
        message = ERR_NOTREGISTERED(this->client->getPrefixe(), check_params(this->client->getNickname()));
        send_message(*this->client, message);
        return;
    }
    if (nb_param == 0) 
    {
        message = this->client->getPrefixe() + " 411 " + this->client->getNickname() + " " + parameters[i] + " :No recipient given (PRIVMSG)\r\n";
        send_message(*this->client, message); 
        return;
    }

    if (this->getArgLine().length() == 0)    
    {
        message = this->client->getPrefixe() + " 412 " + this->client->getNickname() + " " + parameters[i] + " :No text to send\r\n";
        send_message(*this->client, message); 
        return;
    }   
    std::vector<std::string> target = ftsplit(parameters[0], ",");
    int nbtarget = target.size();
    for (i = 0; i < nbtarget; i++)
    {

        if (target[i][0] == '#' || target[i][0] == '&' || target[i][0] == '+' || target[i][0] ==  '!')
        {
            target[i] = lowercase(target[i]); 
            message = this->client->getPrefixe() + "PRIVMSG " +  target[i] + " :" + this->getArgLine() + " \r\n";
            int index = server->getChannelIndex(target[i]);
            if (index == -1) 
            {
                std::string message2 = this->client->getPrefixe() + " 401 " + this->client->getNickname() + " " + target[i] + " :No such nick/channel\r\n";
                send_message(*this->client, message2); 
            } 
            else
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
                
                else
                {
                    std::string message3 = this->client->getPrefixe() + " 404 " + this->client->getPrefixe() + " " + target[i] + " :Cannot send to channel\r\n";
                    send_message(*this->client, message3); 
                }
            }
        }
        else
        {
            int find = 0;
            for (std::map<int, Client>::iterator it = this->server->getClientList().begin(); it!= this->server->getClientList().end(); it++)
            {
                if ((*it).second.getNickname() == target[i])
                {
                    int id = (it)->second.getFd();
                    std::string message = this->client->getPrefixe() + "PRIVMSG " + target[i] + " :" + this->getArgLine() + " \r\n";
                    send(id, message.c_str(), message.size(), 0);
                    find = 1;
                }
            }
            if (find == 0)
            {
                std::string message = this->client->getPrefixe() + " 401 " + this->client->getNickname() + " " + target[i] + " :No such nick/channel\r\n";
                send_message(*this->client, message); 
            }
        }     
    }
}

