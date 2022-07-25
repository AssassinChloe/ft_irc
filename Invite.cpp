/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmercier <vmercier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 11:42:56 by vmercier          #+#    #+#             */
/*   Updated: 2022/07/25 11:44:22 by vmercier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Command.hpp"

void    Command::Invite()
{
    std::string message;
	if (checkRegistration() != 0)
    {
        message = ERR_NOTREGISTERED(this->client->getPrefixe(), check_params(this->client->getNickname()));
        send_message(*this->client, message);
        return;
    }
    if (this->getParameters().size() == 0)
    {
        message = this->client->getPrefixe() + " 461 " + this->getClient().getNickname() + " " + "INVITE :Not enough parameters\r\n"; // ERR_ #461 ERR_NEEDMOREPARAMS
        send_message(*this->client, message);
        return;
    }

    parameters[1] = lowercase(parameters[1]); 
    int index = this->server->getChannelIndex(parameters[1]);
    if (index == -1 || server->getChannel(index).getNbClients() == 0)
    {
        message = this->client->getPrefixe() + " 403 " + this->getClient().getNickname() + " " + parameters[1] + " :No such channel\r\n"; // ERR_NOSUCHCHANNEL (403)
        send_message(*this->client, message);
        return;
    }

    if (server->getChannel(index).isOnChannel(this->getClient().getNickname())) 
    {
        if (server->getChannel(index).isOnChannel(parameters[0]))
        {
            message = this->client->getPrefixe() + " 443 " + parameters[0] + " " + parameters[1] +  " :is already on channel\r\n"; //"<client> <nick> <channel> :is already on channel"
            send_message(*this->client, message); 
            return;
        }
        
        std::string modeChan = server->getChannel(index).getMode();
        if (searchIfMode('i', modeChan) == 1)
        {
            std::string modeClient = client->getChanMode(parameters[1]);
            if (modeClient.size() == 0 || !(searchIfMode('o', modeClient) == 1 || searchIfMode('O', modeClient) == 1 ))
            {
                message =  this->client->getPrefixe() + " 482 " + this->getClient().getNickname() + " " + parameters[1] + " :You're not channel operator\r\n";
                send_message(*this->client, message);
                return;  
            }
        }
        int NickOnServer = 0;
        for (std::map<int, Client>::iterator it = this->server->getClientList().begin(); it != this->server->getClientList().end(); it++)
        {
            if ((*it).second.getNickname() == this->parameters[0])
            {
               NickOnServer = 1;
            } 
        }
        if (NickOnServer == 0) 
        {
            message = this->client->getPrefixe() + " 401 " + this->client->getNickname() + " " + parameters[0] + " :No such nick/channel\r\n";
            send_message(*this->client, message); 
            return;
        }
        server->getChannel(index).addInvited(parameters[0]);
        message =  parameters[0] + " " + parameters[1] + "\r\n"; 
        send_message(*this->client, message); 
        class Client &invited_client = server->getClient(parameters[0]);
        std::string message2 = this->client->getPrefixe() + " INVITE " + parameters[0] + " " + parameters[1] + "\r\n";
        send_message(invited_client, message2);

    }
    else 
    {
        message = this->client->getPrefixe() + " 442 " + this->client->getNickname() + " " + parameters[1] + " :You're not on that channel\r\n";
        send_message(*this->client, message); 
    }
}
