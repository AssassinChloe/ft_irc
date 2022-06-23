#include "Command.hpp"

void Command::welcome()
{
	std::string line;
	std::ifstream input_file("banner_ganesh.txt");
    send(this->client->getFd(), RPL_WELCOME(this->client->getNickname(), this->client->getPrefixe()).c_str(), RPL_WELCOME(this->client->getNickname(), this->client->getPrefixe()).size(), 0);
    send(this->client->getFd(), RPL_YOURHOST(this->client->getPrefixe(), this->client->getNickname(), SERVER_NAME, VERSION).c_str(), RPL_YOURHOST(this->client->getPrefixe(), this->client->getNickname(), SERVER_NAME, VERSION).size(), 0);
    send(this->client->getFd(), RPL_MYINFO(this->client->getPrefixe(), this->client->getNickname(), SERVER_NAME, VERSION, USER_MODE, CHAN_MODE).c_str(), RPL_MYINFO(this->client->getPrefixe(), this->client->getNickname(), SERVER_NAME, VERSION, USER_MODE, CHAN_MODE).size(), 0);
    send(this->client->getFd(), RPL_CREATED(this->client->getPrefixe(), this->client->getNickname(), this->server->getCreation()).c_str(), RPL_CREATED(this->client->getPrefixe(), this->client->getNickname(), this->server->getCreation()).size(), 0);
	if (!input_file.is_open())
        return ;
	//eventuellement mettre ca dans une commande MOTD (RPL 375 372 376) pour afficher le message du jour avec la commande /MOTD	
	while (getline(input_file, line))
	{
		send(this->client->getFd(), (this->client->getPrefixe() + " 372 " + this->client->getNickname() + " :" + line + "\n").c_str(), (this->client->getPrefixe() + " 372 " + this->client->getNickname() + " :" + line + "\n").size(), 0);
	}
}