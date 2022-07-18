#include "Command.hpp"

void Command::oper()
{
    std::string message;
    if (checkRegistration() != 0)
    {
        message = ERR_NOTREGISTERED(this->client->getPrefixe(), check_params(this->client->getNickname()));
        send_message(*this->client, message);
        return;
    }
    if (this->parameters.size() < 2)
    {
        message = this->client->getPrefixe() + " 461 " + this->getClient().getNickname() + " " + "OPER :Not enough parameters\r\n";
        send_message(*this->client, message);
        return ;
    }

    if (this->parameters[0] == OPER_NAME)
    {
        if (this->parameters[1] != OPER_PASS)
        {
            message = ERR_PASSWDMISMATCH(this->client->getPrefixe(), this->client->getNickname());
            send_message(*this->client, message);
            return ;  
        }
        else if (this->client->getHostaddr() != "localhost") //pour le moment toujours vrai...
        {
            message = ERR_NOOPERHOST(this->client->getPrefixe(), this->client->getNickname());
            send_message(*this->client, message);
            return ;
        }
        else
        {
            message = RPL_YOUREOPER(this->client->getPrefixe(), this->client->getNickname());
            this->client->setStatus("o");
            send_message(*this->client, message);
            return ;
        }
    }    
}