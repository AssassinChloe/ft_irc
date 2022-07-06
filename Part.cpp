#include "Command.hpp"

void Command::part()
{
    int index = server->getChannelIndex(parameters[0]);
    std::map<int, Client*>  client_list = server->getChannel(index).getClientMap();
    std::string message = this->client->getPrefixe() + "PART :" + parameters[0] + " " + this->getArgLine() + "\r\n";
    for (std::map<int, Client*>::iterator it = client_list.begin(); it != client_list.end(); it++)
    {
        send_message(*(*it).second, message);
    }
    this->server->getChannel(index).removeClient(*this->client);
    if (this->server->getChannel(index).getNbClients() == 0)
        this->server->delChannel(parameters[0]);
    this->server->getChannel(index).delInvited(this->client->getNickname()); // suppression de la list des invites, a voir si on conserve
    this->client->delChannel(parameters[0]);
}