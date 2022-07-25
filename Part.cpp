#include "Command.hpp"

void Command::part()
{
    std::string message;
	if (checkRegistration() != 0)
    {
        message = ERR_NOTREGISTERED(this->client->getPrefixe(), check_params(this->client->getNickname()));
        send_message(*this->client, message);
        return;
    }
    if (parameters.size() == 0)
    {
        std::string message = this->client->getPrefixe() + " 461 " + this->getClient().getNickname() + " " + "PART :Not enough parameters\r\n"; // ERR_ #461 ERR_NEEDMOREPARAMS
        send_message(*this->client, message);
        return;
    }

    std::vector<std::string> partChan = ftsplit(parameters[0], ",");
    int nbChan = partChan.size();
    for (int i = 0; i < nbChan; i++)
    {
        partChan[i] = lowercase(partChan[i]); // gestion case-sensitivity
        int index = server->getChannelIndex(partChan[i]);
        if (index == -1)
        {
            std::string message = this->client->getPrefixe() + " 403 " + this->getClient().getNickname() + " " + partChan[i] + " :No such channel\r\n"; 
            send_message(*this->client, message); // ERR_NOSUCHCHANNEL (403)
        }
        else
        {
            std::map<int, Client*>  client_list = server->getChannel(index).getClientMap();
            if (server->getChannel(index).isOnChannel(this->client->getNickname())) // client sur le channel
            {
                message = this->client->getPrefixe() + "PART :" + partChan[i] + " " + this->getArgLine() + "\r\n";
                for (std::map<int, Client*>::iterator it = client_list.begin(); it != client_list.end(); it++)
                {
                    send_message(*(*it).second, message);
                }
                this->server->getChannel(index).removeClient(*this->client);
                  
                if (this->server->getChannel(index).getNbClients() == 0)
                {
                    this->server->getChannel(index).delInvited(this->client->getNickname()); // suppression de la list des invites, a voir si on conserve
                    this->server->delChannel(partChan[i]);
                }

              
                this->client->delChannel(partChan[i]);
            }
            else
            {
                message = this->client->getPrefixe() + " 442 "+ this->client->getNickname() + " " + partChan[i] + " :You're not on that channel\r\n";
                send_message(*this->client, message); // ERR_NOTONCHANNEL (442)
            }
        }
    }
}