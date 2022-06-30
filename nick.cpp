/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassassi <cassassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 16:55:18 by cassassi          #+#    #+#             */
/*   Updated: 2022/06/30 16:26:57 by cassassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

//    NICK ERROR
//    ERR_NONICKNAMEGIVEN             
//    ERR_NICKNAMEINUSE               ERR_NICKCOLLISION
//    ERR_UNAVAILRESOURCE             ERR_RESTRICTED

/*
        431    ERR_NONICKNAMEGIVEN
              ":No nickname given"

         - Returned when a nickname parameter expected for a
           command and isn't found.

       432    ERR_ERRONEUSNICKNAME
              "<nick> :Erroneous nickname"

         - Returned after receiving a NICK message which contains
           characters which do not fall in the defined set.  See
           section 2.3.1 for details on valid nicknames.

       433    ERR_NICKNAMEINUSE
              "<nick> :Nickname is already in use"

         - Returned when a NICK message is processed that results
           in an attempt to change to a currently existing
           nickname.

       436    ERR_NICKCOLLISION
              "<nick> :Nickname collision KILL from <user>@<host>"

         - Returned by a server to a client when it detects a
           nickname collision (registered of a NICK that
           already exists by another server).

       437    ERR_UNAVAILRESOURCE
              "<nick/channel> :Nick/channel is temporarily unavailable"

         - Returned by a server to a user trying to join a channel
           currently blocked by the channel delay mechanism.

         - Returned by a server to a user trying to change nickname
           when the desired nickname is blocked by the nick delay
           mechanism.
           
        484    ERR_RESTRICTED
              ":Your connection is restricted!"

         - Sent by the server to a user upon connection to indicate
           the restricted nature of the connection (user mode "+r").
        */
void Command::nick()
{
    std::string charset(NICKNAME_VALID_CHAR);
    std::string rep;
    size_t position;
    
    if (this->parameters.size() == 0 || this->parameters[0].size() > 9)
    {
        send_message(*this->client, ERR_ERRONEUSNICKNAME(this->parameters[0]));
        return ;
    } 
    for (unsigned int i = 0; i < this->parameters[0].size(); i++)
    {
	    if ((position = charset.find(this->parameters[0][i])) == std::string::npos)
        {
            send_message(*this->client, ERR_ERRONEUSNICKNAME(this->parameters[0]));
            return ;
        }
            
    }
    for (std::map<int, Client>::iterator it = this->server->getClientList().begin(); it != this->server->getClientList().end(); it++)
    {
        if ((*it).second.getNickname() == this->parameters[0])
        {
          send_message(*this->client, ERR_NICKNAMEINUSE(this->parameters[0]));
          return ;
        } 
    }
    rep = this->client->getPrefixe() + "NICK " + this->parameters[0] + "\r\n";
    this->client->setNickname(this->parameters[0]);
    send_message(*this->client, rep);
}

void Command::user()
{
    if (this->client->getStatus() != "default")
    {
        send_message(*this->client, ERR_ALREADYREGISTRED(this->client->getPrefixe(), check_params(this->client->getNickname())));
        return ;
    }
    this->client->setUsername(this->parameters[0]);
}

void Command::pass()
{
    if (this->client->getStatus() == "default")
    {
        if (this->parameters[0] == this->server->getPass())
            this->client->setCheckPass(true);
        else
            send_message(*this->client, ERR_PASSWDMISMATCH(this->client->getPrefixe(), check_params(this->client->getNickname())));
    }
}