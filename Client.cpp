/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassassi <cassassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 16:55:32 by cassassi          #+#    #+#             */
/*   Updated: 2022/07/05 10:14:37 by cassassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() {}
Client::Client(struct pollfd newfd)
: username(""), nickname(""), status("default"), hostaddr(HOST), hostname(SERVER_NAME), pass_checked(false), fd(newfd)
{}

Client::~Client() { }

void Client::setStatus(std::string status) { this->status = status; }
void Client::setNickname(std::string nickname) { this->nickname = nickname; }
void Client::setUsername(std::string username) { this->username = username; }
void Client::setCheckPass(bool checked) {this->pass_checked = checked; }
void Client::setLastPing(time_t lastping) { this->last_ping = lastping; }

int Client::getFd() { return fd.fd; }
std::string Client::getStatus() { return status; }
std::string Client::getHostname() { return hostname; }
std::string Client::getHostaddr() { return hostaddr; }
std::string Client::getNickname() { return nickname; }
std::string Client::getUsername() { return username; }

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


//void Client::write(std::string message) { waitingToSend.push_back(message); }
//void Client::sendTo(Client &toClient, std::string message) { toClient.write(":" + " " + message); }
