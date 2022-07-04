#include "Command.hpp"

void Command::oper()
{
    std::string message;
    if (this->parameters.size() < 2)
    {
        message = "OPER :Not enough parameters\r\n";
        send_message(*this->client, message);
        return ;
    }
    if (this->parameters[0] == OPER_NAME)
    {
        std::cout << this->client->getHostaddr() << std::endl;
        if (this->parameters[1] != OPER_PASS)
        {
            message = ERR_PASSWDMISMATCH(this->client->getPrefixe(), this->client->getNickname());
            send_message(*this->client, message);
            return ;  
        }
        else if (this->client->getHostname() != "localhost")
        {
            message = ERR_NOOPERHOST(this->client->getPrefixe(), this->client->getNickname());
            send_message(*this->client, message);
            return ;
        }
        else
        {
            message = RPL_YOUREOPER(this->client->getPrefixe(), this->client->getNickname());
            this->client->setStatus("operator");
            send_message(*this->client, message);
            Command command_line(*this->client, this->server, "MODE +o");
            command_line.execCommand();
            return ;
        }
    }    
}