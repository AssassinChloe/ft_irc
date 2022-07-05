#include "Command.hpp"

void Command::names(int index)
{
    std::string message = this->client->getPrefixe() + "353 " + this->client->getUsername() + " = " + parameters[0] + " :";
    std::map<int, Client*>  client_list = server->getChannel(index).getClientMap();
    for (std::map<int, Client*>::iterator it = client_list.begin(); it != client_list.end(); it++)
    {
        if (searchIfMode(CHAN_USER_MODE, (*it).second->getChanMode(parameters[0])) == 1)
            message = message + "@" + (*it).second->getNickname() + " ";
        // else if (searchIfMode('O', (*it).second->getChanMode(parameters[0])) == 1)
        //     message = message + "~" + (*it).second->getNickname() + " ";
        else
            message = message + (*it).second->getNickname() + " ";
    }
    message = message + "\r\n";
    send_message(*this->client, message);
    message = /*this->client->getPrefixe() + "366 " + this->client->getNickname() + " " +*/ parameters[0] + " :End of /NAMES list\r\n"; // #366 RPL_ENDOFNAMES
    send_message(*this->client, message);
}