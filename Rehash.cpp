#include "Command.hpp"

void    Command::Rehash()
{
    std::string clientStat = this->client->getStatus();
    std::cout << "client status : " << clientStat << std::endl;
    if (searchIfMode('o', clientStat) != 1) // si client n'est pas operateur
    {
        std::string message = this->client->getPrefixe() + " 481 " + this->client->getNickname() + " :Permission Denied- You're not an IRC operator\r\n";
        send_message(*this->client, message);
        return ;
        // :admin!admin@localhost 481 admin :Permission Denied- You're not an IRC operator
    }
    
    if (this->server->init_config(CONFIG_FILE) == -1)
    {
        std::cerr << "error while reading config file" << std::endl;
        return;
    }

    std::string message = this->client->getPrefixe() + " 481 " + CONFIG_FILE + " :Rehashing\r\n";
    send_message(*this->client, message);
    return ;
    // std::string RPL_REHASHING(std::string file) { return file + " :Rehashing"; }
}