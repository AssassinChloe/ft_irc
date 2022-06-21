/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassassi <cassassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 16:55:18 by cassassi          #+#    #+#             */
/*   Updated: 2022/06/21 16:55:19 by cassassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void Command::nick()
{
    std::cout << "set nickname " << this->parameters[0] << std::endl;
    this->client->setNickname(this->parameters[0]);
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