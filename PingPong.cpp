/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PingPong.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmercier <vmercier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 12:37:21 by vmercier          #+#    #+#             */
/*   Updated: 2022/07/25 12:37:52 by vmercier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void Command::Ping()
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
		message = this->client->getPrefixe() + " 461 " + this->getClient().getNickname() + " " + "PING :Not enough parameters\r\n"; 
		send_message(*this->client, message);
		return;
	}
	        
	message = this->getClient().getPrefixe() + "  PONG :" + parameters[0] + " \r\n";
	send_message(*this->client, message);
				
}
