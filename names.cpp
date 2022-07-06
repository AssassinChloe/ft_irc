#include "Command.hpp"

void Command::names(int index)
{
    std::string list = "";
    std::map<int, Client*>  client_list = server->getChannel(index).getClientMap();

    for (std::map<int, Client*>::iterator it = client_list.begin(); it != client_list.end(); it++)
    {
        if (searchIfMode(CHAN_USER_MODE, (*it).second->getChanMode(parameters[0])) == 1)
            list = list + "@" + (*it).second->getNickname() + " ";
        // else if (searchIfMode('O', (*it).second->getChanMode(parameters[0])) == 1)
        //     message = message + "~" + (*it).second->getNickname() + " ";
        else
            list = list + (*it).second->getNickname() + " ";
    }
    std::string message = RPL_NAMREPLY(this->client->getPrefixe(), this->client->getNickname(), parameters[0], list);
    send_message(*this->client, message);
    message = RPL_ENDOFNAMES(this->client->getPrefixe(), this->client->getNickname(), parameters[0]);
    send_message(*this->client, message);
}