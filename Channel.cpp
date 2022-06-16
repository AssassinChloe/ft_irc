#include "Channel.hpp"
#include "Client.hpp"
#include <algorithm>

Channel::Channel()
	: mode("n") {}

void Channel::setName(std::string name) { this->name = name; }
std::string Channel::getName() { return name; }

void Channel::setTopic(std::string topic) { this->topic = topic; }
std::string Channel::getTopic() { return topic; }

void Channel::addClient(Client &client) { clients[client.getFd()] = &client; }
void Channel::removeClient(Client &client) { clients.erase(clients.find(client.getFd())); }
void Channel::removeClient(std::string const &nick)
{
	for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->second->getNickname() == nick)
		{
			clients.erase(it);
			return;
		}
	}
}
std::vector<Client *> Channel::getClients()
{
	std::vector<Client *> clients = std::vector<Client *>();

	for (std::map<int, Client *>::iterator it = this->clients.begin(); it != this->clients.end(); ++it)
		clients.push_back(it->second);
	return clients;
}
bool Channel::isClient(Client &client) { return clients.find(client.getFd()) != clients.end(); }
bool Channel::isOnChannel(std::string const &nick)
{
	for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
		if (it->second->getNickname() == nick)
			return true;
	return false;
}

void Channel::setMode(std::string mode) { this->mode = mode; }
std::string Channel::getMode() { return mode; }

void Channel::setClientMode(Client &client, std::string mode) { client_mode[client.getFd()] = mode; }
std::string Channel::getClientMode(Client &client) { return client_mode[client.getFd()]; }

void Channel::setKey(std::string key) { this->key = key; }
std::string Channel::getKey() { return key; }

void Channel::setMaxClients(std::string max_users) { this->max_clients = max_clients; }
std::string Channel::getMaxClients() { return max_clients; }

void Channel::addInvited(Client &user) { invited.push_back(&user); }
bool Channel::isInvited(Client &user) { return std::find(invited.begin(), invited.end(), &user) != invited.end(); }
void Channel::removeInvited(Client &user)
{
	std::vector<Client *>::iterator it = std::find(invited.begin(), invited.end(), &user);
	if (it != invited.end())
		invited.erase(it);
}

void Channel::broadcast(Client &client, std::string message)
{
	for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
		client.sendTo(*it->second, message);
}

