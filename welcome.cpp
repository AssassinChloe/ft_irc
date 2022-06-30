#include "Command.hpp"

void Command::welcome()
{
	std::string line;
	std::ifstream input_file("banner_ganesh.txt");
    send_message(*this->client, RPL_WELCOME(this->client->getNickname(), this->client->getPrefixe()));
    send_message(*this->client, RPL_YOURHOST(this->client->getPrefixe(), this->client->getNickname(), SERVER_NAME, VERSION));
    send_message(*this->client, RPL_MYINFO(this->client->getPrefixe(), this->client->getNickname(), SERVER_NAME, VERSION, USER_MODE, CHAN_MODE));
    send_message(*this->client, RPL_CREATED(this->client->getPrefixe(), this->client->getNickname(), this->server->getCreation()));
	if (!input_file.is_open())
        return ;
	//eventuellement mettre ca dans une commande MOTD (RPL 375 372 376) pour afficher le message du jour avec la commande /MOTD	
	while (getline(input_file, line))
	{
		std::string message = this->client->getPrefixe() + " 372 " + this->client->getNickname() + " :" + line + "\r\n";
		send_message(*this->client, message);
	}
}