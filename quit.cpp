#include "Command.hpp"

void Command::quit()
{
    // unsigned long i;

    // for (i = 0; i < this->server->getPollFdList().size(); i++)
    // {
    //     if (this->server->getPollFdList()[i].fd == this->client->getFd())
    //     {
            this->server->deleteClient(this->client->getFd());
        // }
    // }
}