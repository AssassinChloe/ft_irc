/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   List.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmercier <vmercier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 11:47:13 by vmercier          #+#    #+#             */
/*   Updated: 2022/07/25 11:49:18 by vmercier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void    Command::List()
{
    std::string message;
	if (checkRegistration() != 0)
    {
        message = ERR_NOTREGISTERED(this->client->getPrefixe(), check_params(this->client->getNickname()));
        send_message(*this->client, message);
        return;
    }
    if (parameters.size() == 0)
    {
        int nbChan = server->getChannelNb();
        for (int i = 0; i < nbChan; i++)
        {
            std::stringstream sstream;
            sstream << server->getChannel(i).getNbClients();
            message = (this->client->getPrefixe()) + " 322 " + (this->client->getNickname()) + " " + server->getChannel(i).getCName();
            message = message + " " + sstream.str() + " :" + server->getChannel(i).getTopic() + "\r\n";
            send_message(*this->client, message);
        }
        std::string message2 = (this->client->getPrefixe()) + " 323 " + (this->client->getNickname()) + " :End of /LIST\r\n";
        send_message(*this->client, message2);
    }

}
