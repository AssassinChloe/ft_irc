/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassassi <cassassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 16:55:18 by cassassi          #+#    #+#             */
/*   Updated: 2022/06/23 12:46:18 by cassassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

//    NICK ERROR
//    ERR_NONICKNAMEGIVEN             
//    ERR_NICKNAMEINUSE               ERR_NICKCOLLISION
//    ERR_UNAVAILRESOURCE             ERR_RESTRICTED
void Command::nick()
{
    std::string charset(NICKNAME_VALID_CHAR);
    std::string rep;
    size_t position;
    if (this->parameters[0].size() > 9)
        return ;
    for (unsigned int i = 0; i < this->parameters[0].size(); i++)
    {
	    if ((position = charset.find(this->parameters[0][i])) == std::string::npos)
        {
            send()
            return ; //432 ERR_ERRONEUSNICKNAME
        }
            
    }
    std::cout << "set nickname " << this->parameters[0] << std::endl;
    rep = this->client->getPrefixe() + "NICK " + this->parameters[0] + "\r\n";
    this->client->setNickname(this->parameters[0]);
    send(this->client->getFd(), rep.c_str(), rep.size(), 0);
}

void Command::user()
{
    std::cout << "set username " << this->parameters[0] << std::endl;
    this->client->setUsername(this->parameters[0]);
}

void Command::pass()
{
    if (this->parameters[0] == this->server->getPass())
    {
        std::cout << "valid password" << std::endl;
        this->client->setCheckPass(true);
    }
}