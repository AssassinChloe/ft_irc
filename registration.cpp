#include "Command.hpp"

int Command::isvalidname()
{
    std::string charset(NICKNAME_VALID_CHAR);
    size_t position;
    
    if (this->parameters.size() == 0 || this->parameters[0].size() > 9)
    {
        send_message(*this->client, ERR_ERRONEUSNICKNAME(this->parameters[0]));
        return (-1);
    } 
    for (unsigned int i = 0; i < this->parameters[0].size(); i++)
    {
	    if ((position = charset.find(this->parameters[0][i])) == std::string::npos)
        {
            send_message(*this->client, ERR_ERRONEUSNICKNAME(this->parameters[0]));
            return (-1);
        }
            
    }
    return (0);
}
void Command::nick()
{
    std::string rep;

    if (isvalidname() < 0)
        return ;
    for (std::map<int, Client>::iterator it = this->server->getClientList().begin(); it != this->server->getClientList().end(); it++)
    {
        if ((*it).second.getNickname() == this->parameters[0])
        {
          send_message(*this->client, ERR_NICKNAMEINUSE(this->parameters[0]));
          return ;
        } 
    }
    rep = this->client->getPrefixe() + "NICK " + this->parameters[0] + "\r\n";
    this->client->setNickname(this->parameters[0]);
    if (this->client->getChanList().size() > 0)
    {
        std::map<std::string, std::string> tmp = this->client->getChanList();
        for (std::map<std::string, std::string>::iterator it = tmp.begin(); it != tmp.end(); it++)
        {
            this->server->getChannel((*it).first).broadcast(rep);
        }
    }
    else
        send_message(*this->client, rep);
}

void Command::user()
{
    if (this->client->getStatus() != UNREGISTERED)
    {
        send_message(*this->client, ERR_ALREADYREGISTRED(this->client->getPrefixe(), check_params(this->client->getNickname())));
        return ;
    }
    this->client->setUsername(this->parameters[0].substr(0, USERLEN));
    if (argLine.size() > 0)
        this->client->setRealname(this->argLine);
}

void Command::pass()
{
    if (this->client->getStatus() == UNREGISTERED)
    {
        if (this->parameters[0] == this->server->getPass())
            this->client->setCheckPass(true);
        else
            send_message(*this->client, ERR_PASSWDMISMATCH(this->client->getPrefixe(), check_params(this->client->getNickname())));
    }
}