#include "Command.hpp"

void Command::oper()
{
    if (this->parameters.size() < 2)
    {
        std::string message = "OPER :Not enough parameters\r\n";
        send(this->client->getFd(), message.c_str(), message.size(), 0);
        return ;
    }
    
}