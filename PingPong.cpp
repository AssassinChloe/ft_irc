#include "Command.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include <ctime>

void Command::Ping()
{
	if (this->getParameters().size() == 0)
		std::cout << "bientot une reply 409" << std::endl;
		// return command->reply(409);
	int fd_client = this->getClient().getFd();
	std::string buffer;
	buffer = (this->getClient().getPrefixe() + "PONG :" + this->getParameters()[0] + "\r\n");
	send(fd_client, buffer.c_str(), buffer.size(), 0);
	std::cout << "fd: "<< fd_client << ", buf Ping: " << buffer << std::endl;
	// command->getClient().sendTo(command->getClient(), "PONG :" + command->getParameters()[0]);
}


void Command::Pong()
{
	if (this->getParameters().size() == 0)
	std::cout << "bientot une reply 409" << std::endl;
		// return command->reply(409);
	this->getClient().setLastPing(std::time(0));
}