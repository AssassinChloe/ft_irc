#include "Command.hpp"

void Command::quit()
{
    std::string message = this->client->getPrefixe() + "QUIT Quit: " + this->argLine + "\r\n";
    if (this->client->getChanList().size() > 0)
    {
        std::map<std::string, std::string> tmp = this->client->getChanList();
        for (std::map<std::string, std::string>::iterator it = tmp.begin(); it != tmp.end(); it++)
        {
            this->server->getChannel((*it).first).broadcast(message);
        }
    }
    this->server->deleteClient(this->client->getFd());
}