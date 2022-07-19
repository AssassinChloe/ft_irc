/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmercier <vmercier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 16:55:32 by cassassi          #+#    #+#             */
/*   Updated: 2022/07/19 11:19:32 by vmercier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"



Client::Client() {}
Client::Client(struct pollfd newfd, Server *ganesh)
: server(ganesh), username(""), nickname(""), status(UNREGISTERED), hostaddr(this->server->getHost()), hostname(this->server->getName()), pass_checked(false), realname("norealname"), fd(newfd)
{}

Client::~Client() { }

void Client::setStatus(std::string status) { this->status = status; }
void Client::setNickname(std::string nickname) { this->nickname = nickname; }
void Client::setUsername(std::string username) { this->username = username; }
void Client::setRealname(std::string realname) {this->realname = realname; }
void Client::setCheckPass(bool checked) {this->pass_checked = checked; }
void Client::setLastPing(time_t lastping) { this->last_ping = lastping; }

int Client::getFd() { return fd.fd; }
std::string Client::getStatus() { return status; }
std::string Client::getHostname() { return hostname; }
std::string Client::getHostaddr() { return hostaddr; }
std::string Client::getNickname() { return nickname; }
std::string Client::getUsername() { return username; }
std::string Client::getRealname() { return realname; }


bool Client::getCheckPass() {return pass_checked; }

std::string Client::getPrefixe()
{
    std::string ret = ":" + check_params(this->nickname) + "!" + check_params(this->username) + "@" + this->hostaddr + " ";
    return (ret);
}
std::map<std::string, std::string> Client::getChanList()
{
    return (this->channel_list);
}

void Client::addChannel(std::string name, std::string mode)
{
    this->channel_list.insert(std::make_pair(name, mode));
}

void Client::delChannel(std::string name)
{
    std::map<std::string, std::string>::iterator ite = this->channel_list.end();
    for(std::map<std::string, std::string>::iterator it = this->channel_list.begin(); it != ite; it++)
    {
        if ((*it).first == name)
        {
            this->channel_list.erase(it);
            return ;
        }
    }
}

std::string Client::getChanMode(std::string name)
{
    return ((*this->channel_list.find(name)).second);
}

void Client::setMode(std::string chan, std::string mode) { this->channel_list[chan] = mode; }

int Client::addMode(std::string chan, char mode)
{
	if (searchIfMode(mode, this->channel_list[chan]) == 1)
		return (0);
	this->channel_list[chan] += mode;
    return (1);
}

int Client::delMode(std::string chan, char mode)
{
	std::string newmode;
	if (searchIfMode(mode, this->channel_list[chan]) == 0)
		return (0);
	for(size_t i = 0; i < this->channel_list[chan].size(); i++)
	{
		if (this->channel_list[chan][i] != mode)
			newmode += this->channel_list[chan][i];
	}
	this->setMode(chan, newmode);
    return (1);
}