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
	std::string buf = (this->getClient().getPrefixe() + "PONG :" + this->getParameters()[0]);
	send(fd_client, buf.c_str(), buf.size(), 0);
	// std::string test = "test";
	// send(fd_client, test.c_str(), test.size(), 0);
	// send(fd_client, (this->getClient().getPrefixe() + " PONG :" + this->getParameters()[0]).c_str(), (" PONG :" + this->getParameters()[0]).size(), 0);
	std::cout << "fd: "<< fd_client << ", buf Ping: " << buf << std::endl;
	// command->getClient().sendTo(command->getClient(), "PONG :" + command->getParameters()[0]);
}


// void PONG(Command *command)
// {
// 	if (command->getParameters().size() == 0)
// 	std::cout << "bientot une reply 409" << std::endl;
// 		// return command->reply(409);
// 	command->getClient().setLastPing(std::time(0));
// }