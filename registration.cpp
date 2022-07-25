/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   registration.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmercier <vmercier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 12:38:58 by vmercier          #+#    #+#             */
/*   Updated: 2022/07/25 12:38:59 by vmercier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

int Command::isvalidname()
{
    std::string charset(NICKNAME_VALID_CHAR);
    size_t position;
    
    if (this->parameters.size() == 0 || this->parameters[0].size() > 9)
    {
        send_message(*this->client, ERR_ERRONEUSNICKNAME(this->parameters[0]));
        return (-1);
    } 
    for (unsigned int i = 0; i < this->parameters[0].size(); i++)
    {
	    if ((position = charset.find(this->parameters[0][i])) == std::string::npos)
        {
            send_message(*this->client, ERR_ERRONEUSNICKNAME(this->parameters[0]));
            return (-1);
        }
            
    }
    return (0);
}
void Command::nick()
{
    std::string message;
    int index;

    if (isvalidname() < 0)
        return ;
    for (std::map<int, Client>::iterator it = this->server->getClientList().begin(); it != this->server->getClientList().end(); it++)
    {
        if ((*it).second.getNickname() == this->parameters[0])
        {
          send_message(*this->client, ERR_NICKNAMEINUSE(this->parameters[0]));
          return ;
        } 
    }
    message = this->client->getPrefixe() + "NICK " + this->parameters[0] + "\r\n";
    this->client->setNickname(this->parameters[0]);
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
    else
        send_message(*this->client, message);
}

void Command::user()
{
    if (this->parameters.size() < 3)
    {
        send_message(*this->client, ERR_NEEDMOREPARAMS(this->client->getPrefixe(), check_params(this->client->getNickname()), "USER "));
        return ;
    }
    if (this->client->getStatus() != UNREGISTERED)
    {
        send_message(*this->client, ERR_ALREADYREGISTRED(this->client->getPrefixe(), check_params(this->client->getNickname())));
        return ;
    }
    this->client->setUsername(this->parameters[0].substr(0, USERLEN));
    if (argLine.size() > 0)
    {
        this->client->setRealname(this->argLine);
    }
}

void Command::pass()
{
    if (this->client->getStatus() == UNREGISTERED)
    {
        if (this->parameters[0] == this->server->getPass())
            this->client->setCheckPass(true);
        else
            send_message(*this->client, ERR_PASSWDMISMATCH(this->client->getPrefixe(), check_params(this->client->getNickname())));
    }
}


void    Command::Cap()
{
    if (parameters[0] == "LS")
    {
        std::string message = this->client->getPrefixe() + " CAP * LS : \r\n";
        send_message(*this->client, message);
        return;
    
    }
}