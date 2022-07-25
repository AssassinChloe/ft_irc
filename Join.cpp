/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmercier <vmercier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 11:46:14 by vmercier          #+#    #+#             */
/*   Updated: 2022/07/25 12:17:02 by vmercier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

bool channelExist(Server *server, std::string chanName)
{
    int nb = server->getChannelNb();

    for (int i = 0; i < nb; i++)
    {
        if (server->getChannelName(i) == chanName)
            return (true);
    }
    return (false);
}

void Command::Join()
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
        message = this->client->getPrefixe() + " 461 " + this->getClient().getNickname() + " " + "JOIN :Not enough parameters\r\n"; // ERR_ #461 ERR_NEEDMOREPARAMS
        send_message(*this->client, message);
        return;
    }
		
	if (this->getParameters()[0] == "0")
    {
        std::string chan;
        std::map<std::string, std::string> tmp = this->client->getChanList();
        for (std::map<std::string, std::string>::iterator it = tmp.begin(); it != tmp.end(); it++)
        {
            chan = "PART " + (*it).first;
            Command command_line(*this->client, this->server, chan);
            command_line.execCommand();
        }
        return;
    }

    std::vector<std::string> joinChan = ftsplit(parameters[0], ",");
    int nbChan = joinChan.size();
    for (int i = 0; i < nbChan; i++)
    {
        std::string LowChan = lowercase(joinChan[i]); 
        if (!channelExist(this->server, LowChan))
            server->addChannel(LowChan); 

        if (channelExist(this->server, LowChan))
        { 
            int index = server->getChannelIndex(LowChan);

            std::string modeChan = server->getChannel(index).getMode();
            int isInvited = 1;
            if (searchIfMode('i', modeChan) == 1)
            {
                std::string modeClientSurServer = client->getStatus();
                if (searchIfMode('o', modeClientSurServer) == 1)
                    isInvited = 1;
                else
                    isInvited = 0;

                int nb = server->getChannel(index).getInvitedNb();
                for (int i=0; i<nb; i++)
                {
                    if (this->client->getNickname() == server->getChannel(index).getInvited(i) )
                        isInvited = 1;
                }
                if (isInvited == 0 )
                {
                    message =  LowChan + " :Cannot join channel (+i)\r\n";
                    send_message(*this->client, message); 
                }
            }
            if (isInvited == 1)
            {
                server->getChannel(index).addClient(this->getClient());
                std::map<int, Client*>  client_list = server->getChannel(index).getClientMap();
                message = this->client->getPrefixe() + "JOIN :" + LowChan + "\r\n" ;
                send_message(*this->client, message);

                if (this->server->getChannel(index).getClients().size() == 1)
                    this->client->addChannel(LowChan, "O");
                else
                    this->client->addChannel(LowChan, "+");

                message = LowChan + " :" + server->getChannel(index).getTopic() + "\r\n";
                send_message(*this->client, message);
                
                std::string list = "";

                for (std::map<int, Client*>::iterator it = client_list.begin(); it != client_list.end(); it++)
                {
                    if (searchIfMode(CHAN_USER_MODE, (*it).second->getChanMode(LowChan)) == 1
                    || searchIfMode(USER_MODE, (*it).second->getStatus()) == 1)
                        list = list + "@" + (*it).second->getNickname() + " ";
                    else
                        list = list + (*it).second->getNickname() + " ";
                }
                for (int j = 0; j < this->server->getChannel(index).getNbClients(); j++)
                {
                    Client tmp = *(this->server->getChannel(index).getClients()[j]);
                    std::string message = RPL_NAMREPLY(tmp.getPrefixe(), tmp.getNickname(), LowChan, list);
                    send_message(tmp, message);
                    message = RPL_ENDOFNAMES(tmp.getPrefixe(), tmp.getNickname(), LowChan);
                    send_message(tmp, message);
                }
                message = this->client->getPrefixe() + "JOIN :" + LowChan + "\r\n" ;
                for (std::map<int, Client*>::iterator it = client_list.begin(); it != client_list.end(); it++)
                {

                    if (this->client != (*it).second)
                        {
                            int id = (*it).second->getFd();
                            send(id, message.c_str(), message.size(), MSG_NOSIGNAL);
                        }
                }
                if (this->server->getChannel(index).getClients().size() == 1)
                    this->client->addChannel(LowChan, "O");
                else if (searchIfMode(USER_MODE, this->client->getStatus()) == 1)
                    this->client->addChannel(LowChan, "o");
                else
                    this->client->addChannel(LowChan, "");
            }

        }
        else 
        {
            std::string message = LowChan + " :Bad Channel Mask\r\n";  
            send_message(*this->client, message);
        }
    }
}
