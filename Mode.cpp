
#include "Command.hpp"

int Command::changeChannelMode(std::string modifier, int index)
{
    int sign = 1;
    std::string message;
    int ret = 0;

    for (size_t i = 0; i < modifier.size(); i++)
    {
        
        if (modifier[i] == '+')
            sign = 1;
        else if (modifier[i] == '-')
            sign = -1;
        else if (searchIfMode(modifier[i], CHAN_MODE) == 1)
        {
            if (sign > 0)
                this->server->getChannel(index).addMode(modifier[i]);
            else
                this->server->getChannel(index).delMode(modifier[i]);          
        }
        else if (searchIfMode(modifier[i], CHAN_USER_MODE) == 1)
        {
            if (modifier[i] == 'o' && this->parameters.size() >= 3)
            {
                if (this->server->getChannel(index).isOnChannel(this->parameters[2]) == true)
                {
                    if (sign > 0)
                        ret = this->server->getClient(this->parameters[2]).addMode(this->parameters[0], modifier[i]);
                    else if (sign < 0 && this->client->getNickname() == this->parameters[2])
                        ret = this->client->delMode(this->parameters[0], modifier[i]);
                    else
                    {
                        message = ERR_USERSDONTMATCH(this->client->getPrefixe(), this->client->getNickname());
                        send_message(*this->client, message);
                        return (ret);
                    }
                }
            }
            else
            {
                message = ERR_NEEDMOREPARAMS(this->client->getPrefixe(), this->client->getNickname(), "MODE");
                send_message(*this->client, message);
                return (ret);
            }
        }
        else
        {
            message = ERR_UNKNOWNMODE(this->client->getPrefixe(), this->client->getNickname(), modifier[i]);
            send_message(*this->client, message);
        }
    }
    return (ret);
}

void    Command::Mode()
{
	std::string message;
	if (checkRegistration() != 0)
    {
        message = ERR_NOTREGISTERED(this->client->getPrefixe(), check_params(this->client->getNickname()));
        send_message(*this->client, message);
        return;
    }
    if (this->parameters.size() == 0)
    {
            message = RPL_UMODEIS(this->client->getPrefixe(), this->client->getNickname(), this->client->getStatus());
            send_message(*this->client, message);
            return;
    }
    parameters[0] = lowercase(parameters[0]); // gestion case-sensitivity
    if (check_if_channel(parameters[0]) == 1)
    {
        int index = this->server->getChannelIndex(this->parameters[0]);
        if (index < 0)
        {
            message = ERR_NOSUCHCHANNEL(this->client->getPrefixe(), this->client->getNickname(), parameters[0]);
            send_message(*this->client, message);
            return ;
        }
        std::string mode = this->server->getChannel(index).getMode(); 
        if (this->parameters.size() == 1)
        {
            message = RPL_CHANNELMODEIS(this->client->getPrefixe(), this->client->getNickname(), parameters[0], mode, ""); //dernier argument je sais pas ce qu'on doit/peut y mettre
            send_message(*this->client, message);
            return;
        }
        if (searchIfMode(CHAN_USER_MODE, this->client->getChanMode(this->parameters[0])) == 0 
            && searchIfMode(USER_MODE, this->client->getStatus()) == 0)
        {
            message = ERR_CHANOPRIVSNEEDED(this->client->getPrefixe(), this->client->getNickname(), parameters[0]);
            send_message(*this->client, message);
            return;
        }
        if (changeChannelMode(this->parameters[1], index) == 1)
        {
            std::string list = "";
            std::map<int, Client*>  client_list = server->getChannel(index).getClientMap();

            for (std::map<int, Client*>::iterator it = client_list.begin(); it != client_list.end(); it++)
            {
                if (searchIfMode(CHAN_USER_MODE, (*it).second->getChanMode(parameters[0])) == 1
                || searchIfMode(USER_MODE, (*it).second->getStatus()) == 1)
                    list = list + "@" + (*it).second->getNickname() + " ";
                else
                    list = list + (*it).second->getNickname() + " ";
            }
            for (int i = 0; i < this->server->getChannel(index).getNbClients(); i++)
            {
                Client tmp = *(this->server->getChannel(index).getClients()[i]);
                message = RPL_NAMREPLY(tmp.getPrefixe(), tmp.getNickname(), parameters[0], list);
                send_message(tmp, message);
                message = RPL_ENDOFNAMES(tmp.getPrefixe(), tmp.getNickname(), parameters[0]);
                send_message(tmp, message);
            }
        }
        if (this->server->getChannel(index).getMode() != mode)
        {
            for(int i = 0; i < this->server->getChannel(index).getNbClients(); i++)
            {
                Client tmp = *(this->server->getChannel(index).getClients()[i]);
                message = RPL_CHANNELMODEIS(tmp.getPrefixe(), tmp.getNickname(), parameters[0], this->server->getChannel(index).getMode(), "");
                send_message(tmp, message);
            }
        }
    }
    else
    {
        int find = 0;
        int sign = 1;
        for (std::map<int, Client>::iterator it = this->server->getClientList().begin(); it!= this->server->getClientList().end(); it++)
        {
            if ((*it).second.getNickname() == parameters[0])
            {
                find = 1;
                if(this->client->getNickname() != parameters[0])
                {
                    message = ERR_USERSDONTMATCH(this->client->getPrefixe(), this->client->getNickname());
                    send_message(*this->client, message);
                    return ;
                }
                for (size_t i = 0; i < parameters[1].size(); i++)
                {
                    if (parameters[1][i] == '+')
                        sign = 1;
                    else if (parameters[1][i] == '-')
                        sign = -1;
                    else if (parameters[1][i] == 'o')
                    {
                        if (sign < 0)
                        {
                            this->client->setStatus(GANESH_FRIEND);
                            message = RPL_UMODEIS(this->client->getPrefixe(), this->client->getNickname(), this->client->getStatus());
                            send_message(*this->client, message);
                            return;
                        } 
                    }
                    else
                    {
                        message = ERR_UNKNOWNMODE(this->client->getPrefixe(), this->client->getNickname(), parameters[1][i]);
                        send_message(*this->client, message);
                    }
                }
                return ;
            }
        }
        if (find == 0)
        {
            std::string message = ERR_NOSUCHNICK(this->client->getPrefixe(), this->client->getNickname(), parameters[0]);
            send_message(*this->client, message);
        }
    }
}