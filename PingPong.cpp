#include "Command.hpp"

void Command::Ping()
{
	if (this->getParameters().size() == 0)
	{
		std::string message = this->getClient().getPrefixe() + " :No origin specified\r\n";
		send_message(*this->client, message);
		return;
	}
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
	{
		std::string message = this->getClient().getPrefixe() + " :No origin specified\r\n";
		send_message(*this->client, message);
		return;
	}
	this->getClient().setLastPing(std::time(0));
}


// ERR_NOORIGIN (409)
//   "<client> :No origin specified"
// Indicates a PING or PONG message missing the originator parameter which is required by old IRC servers. 
// Nowadays, this may be used by some servers when the PING <token> is empty.