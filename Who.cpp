// 5 > :vmercier_!vmercier@localhost 352 vmercier_ #test3 vmercier localhost  vmercier H :0 realname
// 5 > :vmercier_!vmercier@localhost 352 vmercier_ #test3 vmercier localhost  vmercier_ H :0 realname
// 5 > :vmercier_!vmercier@localhost 315 vmercier_ vmercier :End of /WHO list
//std::string RPL_WHOREPLY(std::string channel, std::string user, std::string host, std::string server, std::string nick, std::string state, std::string real) 
//  { return channel + " " + user + " " + host + " " + server + " " + nick + " " + state + " :0 " + real; }

#include "Command.hpp"

void    Command::Who()
{
    std::string message;
    std::string flag;
    if (this->parameters.size() > 0) 
    {
        if (check_if_channel(this->parameters[0]) == 1)
        {
            int index = this->server->getChannelIndex(this->parameters[0]);

            if (index >= 0)
            {
                std::map<int, Client*>  client_list = server->getChannel(index).getClientMap();
                for (std::map<int, Client*>::iterator it = client_list.begin(); it != client_list.end(); it++)
                {
                    flag = "H";
                    if (searchIfMode(CHAN_USER_MODE, (*(*it).second->getChanList().find(parameters[0])).second) == 1)
                        flag += "@";
                    message = RPL_WHOREPLY(this->client->getPrefixe(), this->client->getNickname(), parameters[0], (*it).second->getUsername(), (*it).second->getHostaddr(), (*it).second->getHostname(), (*it).second->getNickname(), flag);
                    send_message(*this->client, message);
                }
            }   
        }
        else
        {
            flag = "H";
            for (std::map<int, Client>::iterator it = this->server->getClientList().begin(); it!= this->server->getClientList().end(); it++)
            {
                if ((*it).second.getNickname() == this->parameters[0])
                {
                    if (searchIfMode(USER_MODE, (*it).second.getStatus()) == 1)
                        flag += "*";
                    message = RPL_WHOREPLY(this->client->getPrefixe(), this->client->getNickname(), parameters[0], (*it).second.getUsername(), (*it).second.getHostaddr(), (*it).second.getHostname(), (*it).second.getNickname(), flag);
                    send_message(*this->client, message);
                    message = RPL_ENDOFWHO(this->client->getPrefixe(), this->client->getNickname(), parameters[0]);
                    send_message(*this->client, message);
                    return ;
                }
            }
        }
    }
    message = RPL_ENDOFWHO(this->client->getPrefixe(), this->client->getNickname(), parameters[0]);
    send_message(*this->client, message);
    return;
}

// rajouter  user mode !!!

// < WHO #test
// 4 > :lilin!liliu@localhost 352 lilin #test liliu localhost  lilin H :0 realname
// 4 > :lilin!liliu@localhost 352 lilin #test lalau localhost  lalan H :0 realname
// 4 > :lilin!liliu@localhost 352 lilin #test admin localhost  admin H@ :0 realname
    