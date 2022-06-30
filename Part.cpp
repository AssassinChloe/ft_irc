#include "Command.hpp"

void Command::part()
{
    int index = server->getChannelIndex(parameters[0]);
    std::map<int, Client*>  client_list = server->getChannel(index).getClientMap();
    std::string message = this->client->getPrefixe() + "PART :" + parameters[0] + " " + this->getArgLine() + "\r\n";
    for (std::map<int, Client*>::iterator it = client_list.begin(); it != client_list.end(); it++)
    {
        send((*it).second->getFd(), message.c_str(), message.size(), 0);
    }
    this->server->getChannel(index).removeClient(*this->client);
    if (this->server->getChannel(index).getChannelSize() == 0)
        this->server
    this->client->delChannel(parameters[0]);
}