#include "Command.hpp"

void Command::quit()
{
    std::cout << this->client->getNickname() << " quit the server" << std::endl;
    if (this->client->getChanList().size() > 0)
    {
        Command command_line(*this->client, this->server, "JOIN 0");
        command_line.execCommand();
    }
    close(this->client->getFd());
    if (this->server->getClientList().size() > 0)
        this->server->getClientList().erase(this->client->getFd());
    for (std::vector<struct pollfd>::iterator it = this->server->getPollFdList().begin(); it != this->server->getPollFdList().end(); it++)
    {
        if ((*it).fd == this->client->getFd())
        {
            this->server->getPollFdList().erase(it);
            return ;
        }
    }
}