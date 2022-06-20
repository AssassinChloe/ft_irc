#include "Client.hpp"

Client::Client() {}
Client::Client(struct pollfd newfd) : pass_checked(false), fd(newfd)
{  }

Client::~Client() { }

void Client::setStatus(std::string status) { this->status = status; }
void Client::setNickname(std::string nickname) { this->nickname = nickname; }
void Client::setUsername(std::string username) { this->username = username; }
void Client::setRealname(std::string realname) { this->realname = realname; }
void Client::setCheckPass(bool checked) {this->pass_checked = checked; }

int Client::getFd() { return fd.fd; }
std::string Client::getStatus() { return status; }
std::string Client::getHostname() { return hostname; }
std::string Client::getNickname() { return nickname; }
std::string Client::getUsername() { return username; }
std::string Client::getRealname() { return realname; }
bool Client::getCheckPass() {return pass_checked; }

//void Client::write(std::string message) { waitingToSend.push_back(message); }
//void Client::sendTo(Client &toClient, std::string message) { toClient.write(":" + " " + message); }