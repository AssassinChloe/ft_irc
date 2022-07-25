/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   welcome.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassassi <cassassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 11:36:48 by cassassi          #+#    #+#             */
/*   Updated: 2022/07/25 11:36:49 by cassassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void Command::welcome()
{
	std::string line;
	std::ifstream input_file(this->server->getBanner());
    send_message(*this->client, RPL_WELCOME(this->client->getNickname(), this->client->getPrefixe()));
    send_message(*this->client, RPL_YOURHOST(this->client->getPrefixe(), this->client->getNickname(), this->server->getName(), this->server->getVersion()));
    send_message(*this->client, RPL_MYINFO(this->client->getPrefixe(), this->client->getNickname(), this->server->getName(), this->server->getVersion(), USER_MODE, CHAN_MODE + CHAN_USER_MODE));
    send_message(*this->client, RPL_CREATED(this->client->getPrefixe(), this->client->getNickname(), this->server->getCreation()));
	if (!input_file.is_open())
        return ;
	while (getline(input_file, line))
	{
		std::string message = this->client->getPrefixe() + " 372 " + this->client->getNickname() + " :" + line + "\r\n";
		send_message(*this->client, message);
	}
}