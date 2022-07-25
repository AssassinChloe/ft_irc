/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassassi <cassassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 11:42:59 by cassassi          #+#    #+#             */
/*   Updated: 2022/07/25 11:43:03 by cassassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void    Command::Topic()
{
	std::string message;
	if (checkRegistration() != 0)
    {
        message = ERR_NOTREGISTERED(this->client->getPrefixe(), check_params(this->client->getNickname()));
        send_message(*this->client, message);
        return;
    }
    int nb_param = parameters.size();

    if (nb_param == 0) 
    {
        message = this->client->getPrefixe() + " 461 " + this->getClient().getNickname() + " " + "TOPIC :Not enough parameters\r\n";
        send_message(*this->client, message);
        return;
    }
    
    else
    {
        std::string LowChan = lowercase(parameters[0]);
        int index = server->getChannelIndex(LowChan);
        if (index == -1)
        {
            message = this->client->getPrefixe() + " 403 " + this->getClient().getNickname() + " " + LowChan + " :No such channel\r\n";
            send_message(*this->client, message);
            return;
        }
        if (server->getChannel(index).isOnChannel(this->getClient().getNickname()))
        {
            std::string modeChan = server->getChannel(index).getMode();
            if (searchIfMode('t', modeChan) == 1)
            {
                std::string modeClient = server->getChannel(index).getClientMode(this->getClient());
                if (!(searchIfMode('o', modeClient) == 1 || searchIfMode('O', modeClient) == 1 ))
                {
                    std::string message =  this->client->getPrefixe() + " 482 " + this->getClient().getNickname() + " " + LowChan + " :You're not channel operator\r\n";
                    send_message(*this->client, message);
                    return;
                }
            }
            if (nb_param == 1 && argLine == "")
            {
                if (server->getChannel(index).getTopic().length() != 0)
                {
                    message = LowChan + " :" + server->getChannel(index).getTopic() + "\r\n";
                    send_message(*this->client, message);
                }
                else 
                {
                    std::string message = LowChan + " :No topic is set\r\n";
                    send_message(*this->client, message);
                }
            }
            if (nb_param == 1 && argLine.length() != 0)
            {
                if (TOPIC_MAX_LEN > 0)
                {
                    if (argLine.size() >= TOPIC_MAX_LEN)
                        argLine.resize(TOPIC_MAX_LEN);
                }
                server->getChannel(index).setTopic(argLine);
                server->getChannel(index).setTopicSetter(this->client);
                server->getChannel(index).setLastTopicSet();
                std::stringstream ss;
                ss << server->getChannel(index).getLastTopicSet();
                std::string timestring = ss.str();
                std::string message = this->client->getPrefixe() +" 333 "+ " server " + LowChan + " " ;
                message = message + server->getChannel(index).getTopicSetter()->getNickname() + " " ;
                message = message + timestring +  "\r\n";
                send_message(*this->client, message);

                std::map<int, Client*>  client_list = server->getChannel(index).getClientMap();
                for (std::map<int, Client*>::iterator it = client_list.begin(); it != client_list.end(); it++)
                {
                        std::string message = this->client->getPrefixe() + " TOPIC "+ LowChan + " :" + server->getChannel(index).getTopic() + "\r\n";
                        int id = (*it).second->getFd();
                        send(id, message.c_str(), message.size(), 0);
                }
            }
        }

        else // client pas sur le channel
        {
            std::string message = this->client->getPrefixe() +" 442 "+ this->client->getNickname() + " " + LowChan + " :You're not on that channel\r\n";
            send_message(*this->client, message);
        }
    }
}