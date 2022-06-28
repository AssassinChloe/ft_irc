#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "ft_irc.hpp"
#include <algorithm>


// Channels names are strings (beginning with a '&', '#', '+' or '!'
//    character) of length up to fifty (50) characters.  Channel names are
//    case insensitive.

//    Apart from the the requirement that the first character being either
//    '&', '#', '+' or '!' (hereafter called "channel prefix"). The only
//    restriction on a channel name is that it SHALL NOT contain any spaces
//    (' '), a control G (^G or ASCII 7), a comma (',' which is used as a
//    list item separator by the protocol).  Also, a colon (':') is used as
//    a delimiter for the channel mask.  The exact syntax of a channel name
//    is defined in "IRC Server Protocol" 

int Channel::formatName(std::string name)
{
	int length = name.length();
	if (length < 2 || length > 50)
		return (0);

	if (name[0] != '#' && name[0] != '&' && name[0] != '+' && name[0] != '!')
		return (0);

	for (int i = 1; i< length; i++)
	{
		if (name[i] == ' ' || (name[i] == '^' && name[i+1] && name[i+1] == 'G') 
			|| name[i] == ',' || name[i] == ':')
			return (0);
	}
	return (1);
}

Channel::Channel(std::string namechan) //(Server *server, std::string namechan)
	: topic("en attente de la sagesse de Ganesh pour developper le topic"), mode("n")
	{
		if (formatName(namechan))
		{
			name = namechan;
			std::cout << "channel added to chan list of server " << std::endl;
		}
		else
			std::cout << "bad channel name" << std::endl;
	}

// void Channel::setName(std::string name) { this->name = name; }
Channel::~Channel() { }

Channel::Channel(const Channel &src)
{
    *this = src;
    return;
}

Channel &      Channel::operator=(Channel const &rhs)
{
    if(this != &rhs)
    {
        this->name = rhs.name;
        this->topic = rhs.topic;
		this->clients = rhs.clients;
		this->mode = rhs.mode;
		this->client_mode = rhs.client_mode;
		this->max_clients = rhs.max_clients;
    }
    return *this;
}


std::string Channel::getCName() { return name; }

void Channel::setTopic(std::string topic) { this->topic = topic; }
std::string Channel::getTopic() { return topic; }

int Channel::getNbClients(){
return this->clients.size();
};

void Channel::addClient(Client &client) { 
	// if (this->clients.size() < max_clients)
		std::cout << "entree dans add client" << std::endl;
		clients[client.getFd()] = &client; 
		int nb = getNbClients();
		std::cout << "mb cleints dans channel" << nb << std::endl;

	// else
	// 	std::cout << "max number of clients on " << name << " channel reached" << std::endl;
}
void Channel::removeClient(Client &client) 
{ 
	clients.erase(clients.find(client.getFd()));
	// rajouter un check pour voir s'il n'y a plus de client -> effcaer le channel ?
}
void Channel::removeClient(std::string const &nick) // idem ci dessus
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

std::map<int, Client*> Channel::getClientMap() { return clients;}

// void Channel::setKey(std::string key) { this->key = key; }
// std::string Channel::getKey() { return key; }

// void Channel::setMaxClients(int max_users) { this->max_clients = max_clients; }
// int Channel::getMaxClients() { return max_clients; }

// void Channel::addInvited(Client &user) { invited.push_back(&user); }
// bool Channel::isInvited(Client &user) { return std::find(invited.begin(), invited.end(), &user) != invited.end(); }
// void Channel::removeInvited(Client &user)
// {
// 	std::vector<Client *>::iterator it = std::find(invited.begin(), invited.end(), &user);
// 	if (it != invited.end())
// 		invited.erase(it);
// }

// void Channel::broadcast(Client &client, std::string message)
// {

// //get number of clients du chnnel
// // boucle for pour chaque client du channel:
// // 	si le client est different du clientBroadcast (?), alors envoi du message
// }


