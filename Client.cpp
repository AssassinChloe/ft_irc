#include "Client.hpp"

Client::~Client() { close(fd); }

void Client::setStatus(std::string status) { this->status = status; }
void Client::setNickname(std::string nickname) { this->nickname = nickname; }
void Client::setUsername(std::string username) { this->username = username; }
void Client::setRealname(std::string realname) { this->realname = realname; }

int Client::getFd() { return fd.fd; }
std::string Client::getStatus() { return status; }
std::string Client::getHostname() { return hostname; }
std::string Client::getNickname() { return nickname; }
std::string Client::getUsername() { return username; }
std::string Client::getRealname() { return realname; }

//void Client::write(std::string message) { waitingToSend.push_back(message); }
//void Client::sendTo(Client &toClient, std::string message) { toClient.write(":" + " " + message); }