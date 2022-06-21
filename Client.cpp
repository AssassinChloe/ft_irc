/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassassi <cassassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 16:55:32 by cassassi          #+#    #+#             */
/*   Updated: 2022/06/21 16:56:13 by cassassi         ###   ########.fr       */
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
std::string Client::getNickname() { return nickname; }
std::string Client::getUsername() { return username; }

bool Client::getCheckPass() {return pass_checked; }
std::string Client::getPrefixe()
{
    std::string ret = ":" + this->nickname + "!" + this->username + "@" + this->hostaddr + " ";
    return (ret);
}
//void Client::write(std::string message) { waitingToSend.push_back(message); }
//void Client::sendTo(Client &toClient, std::string message) { toClient.write(":" + " " + message); }
