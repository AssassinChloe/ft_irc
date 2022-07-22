#include "Command.hpp"

void    Command::Who()
{
    std::string message;
    std::string flag;
	if (checkRegistration() != 0)
    {
        message = ERR_NOTREGISTERED(this->client->getPrefixe(), check_params(this->client->getNickname()));
        send_message(*this->client, message);
        return;
    }
    if (this->parameters.size() > 0) 
    {
        std::string LowChan = lowercase(parameters[0]); // gestion case-sensitivity
        if (check_if_channel(LowChan) == 1)
        {
            int index = this->server->getChannelIndex(LowChan);

            if (index >= 0)
            {
                std::map<int, Client*>  client_list = server->getChannel(index).getClientMap();
                for (std::map<int, Client*>::iterator it = client_list.begin(); it != client_list.end(); it++)
                {
                    flag = "H";
                    if (searchIfMode(USER_MODE, (*it).second->getStatus()) == 1)
                        flag += "*";
                    else if (searchIfMode(CHAN_USER_MODE, (*(*it).second->getChanList().find(LowChan)).second) == 1)
                        flag += "@";
                    message = RPL_WHOREPLY(this->client->getPrefixe(), this->client->getNickname(), LowChan, (*it).second->getUsername(), (*it).second->getHostaddr(), (*it).second->getHostname(), (*it).second->getNickname(), flag, this->client->getRealname());
                    send_message(*this->client, message);
                }
            }   
        }
        else
        {
            flag = "H";
            for (std::map<int, Client>::iterator it = this->server->getClientList().begin(); it!= this->server->getClientList().end(); it++)
            {
                if ((*it).second.getNickname() == LowChan)
                {
                    if (searchIfMode(USER_MODE, (*it).second.getStatus()) == 1)
                        flag += "*";
                    message = RPL_WHOREPLY(this->client->getPrefixe(), this->client->getNickname(), LowChan, (*it).second.getUsername(), (*it).second.getHostaddr(), (*it).second.getHostname(), (*it).second.getNickname(), flag, this->client->getRealname());
                    send_message(*this->client, message);
                }
            }
        }
    }
    std::string LowChan = lowercase(parameters[0]); // gestion case-sensitivity
    message = RPL_ENDOFWHO(this->client->getPrefixe(), this->client->getNickname(), LowChan);
    send_message(*this->client, message);
    return;
}