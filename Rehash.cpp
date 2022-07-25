/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Rehash.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmercier <vmercier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 12:39:06 by vmercier          #+#    #+#             */
/*   Updated: 2022/07/25 12:39:13 by vmercier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void    Command::Rehash()
{
    std::string clientStat = this->client->getStatus();
    if (searchIfMode('o', clientStat) != 1) 
    {
        std::string message = this->client->getPrefixe() + " 481 " + this->client->getNickname() + " :Permission Denied- You're not an IRC operator\r\n";
        send_message(*this->client, message);
        return ;
    }
    
    if (this->server->init_config(CONFIG_FILE) == -1)
    {
        std::cerr << "error while reading config file" << std::endl;
        return;
    }

    std::string message = this->client->getPrefixe() + " 481 " + CONFIG_FILE + " :Rehashing\r\n";
    send_message(*this->client, message);
    return ;
}