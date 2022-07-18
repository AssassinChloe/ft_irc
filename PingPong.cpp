#include "Command.hpp"

void Command::Ping()
{
	std::string message;
	if (checkRegistration() != 0)
    {
        message = ERR_NOTREGISTERED(this->client->getPrefixe(), check_params(this->client->getNickname()));
        send_message(*this->client, message);
        return;
    }
	if (this->getParameters().size() == 0)
	{
		message = this->getClient().getPrefixe() + " :No origin specified\r\n";
		send_message(*this->client, message);
		return;
	}
	message = this->getClient().getPrefixe() + "PONG :" + this->getParameters()[0] + "\r\n";
	send_message(*this->client, message);
}

//A SUPPRIMER ? 
// void Command::Pong()
// {
// 	if (this->getParameters().size() == 0)
// 	{
// 		std::string message = this->getClient().getPrefixe() + " :No origin specified\r\n";
// 		send_message(*this->client, message);
// 		return;
// 	}
// 	this->getClient().setLastPing(std::time(0));
// }