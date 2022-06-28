// 5 > :vmercier_!vmercier@localhost 352 vmercier_ #test3 vmercier localhost  vmercier H :0 realname
// 5 > :vmercier_!vmercier@localhost 352 vmercier_ #test3 vmercier localhost  vmercier_ H :0 realname
// 5 > :vmercier_!vmercier@localhost 315 vmercier_ vmercier :End of /WHO list
//std::string RPL_WHOREPLY(std::string channel, std::string user, std::string host, std::string server, std::string nick, std::string state, std::string real) 
//  { return channel + " " + user + " " + host + " " + server + " " + nick + " " + state + " :0 " + real; }

#include "Command.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "ft_irc.hpp"


void    Command::Who()
{
    if (this->getParameters().size() == 0) 
        return;

    if (this->getParameters().size() == 1) // a modifier pour user mode ou channel mode
    {
        // a changer si le mode du client du channel peut etre modifie
        // std::string message = this->client->getPrefixe() + "352 " +  this->client->getNickname() + parameters[0];
        int index = server->getChannelIndex(parameters[0]);
        std::map<int, Client*>  client_list = server->getChannel(index).getClientMap();
        for (std::map<int, Client*>::iterator it = client_list.begin(); it != client_list.end(); it++)
        {
            std::string message = this->client->getPrefixe() + "352 " +  this->client->getNickname() + " " + parameters[0] + " ";
            message = message + (*it).second->getUsername() + " " +(*it).second->getHostaddr()+ " " + (*it).second->getNickname() + " H :0 realname" +" \r\n";
            // attention a changer avec mode client (H par default, si mode a alors G), + realname ou name server 
            std::cout << "----message----" << message << std::endl;
            send_message(*this->client, message);
        }
           
        std::string message2 = this->client->getPrefixe() + "315 " +  this->parameters[0] + /*this->client->getNickname() + " " + this->client->getUsername() +*/ " :End of /WHO list";
        send_message(*this->client, message2);
        std::cout << "----message----" << message2 << std::endl;
        return;
    }
    return;
}

// rajouter  user mode !!!
    