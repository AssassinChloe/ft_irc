#include "Channel.hpp"

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
		if (name[i] == ' ' || (name[i] == 7) 
			|| name[i] == ',' || name[i] == ':')
			return (0);
	}
	return (1);
}

Channel::Channel(std::string namechan) //(Server *server, std::string namechan)
// : mode("n") // si on veut tester sans mettre un topic par defaut
: topic("en attente de la sagesse de Ganesh pour developper le topic"), mode("n")
	{
		if (formatName(namechan))
			name = namechan;

	}

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

int Channel::getNbClients()
{
	return this->clients.size();
}

void Channel::addClient(Client &client) { clients[client.getFd()] = &client; }

void Channel::removeClient(Client &client) 
{
	clients.erase(clients.find(client.getFd()));

	// rajouter un check pour voir s'il n'y a plus de client -> effacer le channel ?
	//check fait dans remove client du serveur. a deplacer ici? -> fait dans la partie kick et part
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
	std::vector<Client *> clients = std::vector<Client *>(); // ligne pas == a std::vector<Client *> clients; ??

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

void Channel::addMode(char mode)
{
	if (searchIfMode(mode, this->mode) == 1)
		return ;
	this->mode += mode;
}

void Channel::delMode(char mode)
{
	std::string newmode;
	if (searchIfMode(mode, this->mode) == 0)
		return ;
	for(size_t i = 0; i < this->mode.size(); i++)
	{
		if (this->mode[i] != mode)
			newmode += this->mode[i];
	}
	this->setMode(newmode);
}

void Channel::setClientMode(Client &client, std::string mode) { client_mode[client.getFd()] = mode; }

std::string Channel::getClientMode(Client &client) { return client_mode[client.getFd()]; }

std::map<int, Client*> Channel::getClientMap() { return clients;}

Client 	*Channel::getTopicSetter() {return this->topicSetter;}

void 	Channel::setTopicSetter(Client *client) {this->topicSetter = client;}

time_t  Channel::getLastTopicSet(){ return this->lastTopicSet;}

void    Channel::setLastTopicSet(){ this->lastTopicSet = std::time(0);}

void	Channel::broadcast(std::string message)
{
    for (std::map<int, Client *>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
	{	
		send_message(*(*it).second, message);
	}
}

void Channel::addInvited(std::string nickname)
{
    if (nickname.length() != 0)
    {
        if (getInvitedIndex(nickname) == -1)
		{
			invited.push_back(nickname);
		}
    }
}

void Channel::delInvited(std::string nickname)
{
	int nb = invited.size();
	for (int i = 0; i < nb; i++)
	{
		if (invited[i] == nickname)
		{
			invited.erase(invited.begin() + i);
			return;
		}
	}
}

std::string Channel::getInvited(int i)
{
	return(invited[i]);
}

int Channel::getInvitedIndex(std::string nickname)
{
	int nb = invited.size();
	for (int i = 0; i < nb; i++)
    {
        if (invited[i] == nickname)
            return (i);
    }
    return (-1); 

}

int Channel::getInvitedNb()
{
	return (invited.size());
}

