/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oper.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmercier <vmercier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 12:35:29 by vmercier          #+#    #+#             */
/*   Updated: 2022/07/25 12:35:35 by vmercier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void Command::oper()
{
    std::string message;
    if (checkRegistration() != 0)
    {
        message = ERR_NOTREGISTERED(this->client->getPrefixe(), check_params(this->client->getNickname()));
        send_message(*this->client, message);
        return;
    }
    if (this->parameters.size() < 2)
    {
        message = this->client->getPrefixe() + " 461 " + this->getClient().getNickname() + " " + "OPER :Not enough parameters\r\n";
        send_message(*this->client, message);
        return ;
    }

    if (this->parameters[0] == this->server->getOperName())
    {
        if (this->parameters[1] != this->server->getOperPass())
        {
            message = ERR_PASSWDMISMATCH(this->client->getPrefixe(), this->client->getNickname());
            send_message(*this->client, message);
            return ;  
        }
        else if (this->client->getHostaddr() != "localhost") 
        {
            message = ERR_NOOPERHOST(this->client->getPrefixe(), this->client->getNickname());
            send_message(*this->client, message);
            return ;
        }
        else
        {
            message = RPL_YOUREOPER(this->client->getPrefixe(), this->client->getNickname());
            this->client->setStatus("o");
            send_message(*this->client, message);
            return ;
        }
    }    
}