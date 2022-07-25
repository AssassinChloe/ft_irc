
#include "Command.hpp"

void    Command::Kill()
{
        
    int nb_param = parameters.size();
    std::string message;
	if (checkRegistration() != 0)
    {
        message = ERR_NOTREGISTERED(this->client->getPrefixe(), check_params(this->client->getNickname()));
        send_message(*this->client, message);
        return;
    }
    if (nb_param <= 0) 
    {
        message =this->client->getPrefixe() + " 461 " + this->getClient().getNickname() + " " + "KILL :Not enough parameters\r\n"; // ERR_NEEDMOREPARAMS (461)
        send_message(*this->client, message);
        return;
    }
    
    else
    {
        std::string clientStat = this->client->getStatus();
        if (searchIfMode('o', clientStat) != 1) 
        {
            message = this->client->getPrefixe() + " 481 " + this->client->getNickname() + " :Permission Denied- You're not an IRC operator\r\n";
            send_message(*this->client, message);
        }
        else
        {
            int NickOnServer = 0;
            for (std::map<int, Client>::iterator it = this->server->getClientList().begin(); it != this->server->getClientList().end(); it++)
            {
                if ((*it).second.getNickname() == this->parameters[0])
                {
                    NickOnServer = 1;
                } 
            }

            if (NickOnServer == 0) 
            {
                std::string message = this->client->getPrefixe() + " 401 " + this->client->getNickname() + " " + parameters[0] + " :No such nick/channel\r\n";
                send_message(*this->client, message); 
                return;
            }

            int id = server->getClient(parameters[0]).getFd();
            message = server->getClient(id).getPrefixe()  + " QUIT " + parameters[0] + " :"+ argLine + "\r\n";
            send_message(*this->client, message);
            std::string message2 = this->client->getPrefixe() + " KILL " + parameters[0] + " :" + argLine + "\r\n";
            send_message(server->getClient(parameters[0]), message2);
            server->deleteClient(id);

        }
    }
}

