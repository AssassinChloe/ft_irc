#include "Command.hpp"
#include <string>
#include <sstream>
#include <cstdlib> 



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
		// message = this->getClient().getPrefixe() + " :No origin specified\r\n";
		message = this->client->getPrefixe() + " 461 " + this->getClient().getNickname() + " " + "PING :Not enough parameters\r\n"; // ERR_ #461 ERR_NEEDMOREPARAMS
		send_message(*this->client, message);
		return;
	}
	            // std::stringstream ss;
                // ss << std::time(0) ; //- parameters[0];
                // std::string timestring = ss.str();
				// long nb = atol(parameters[0].c_str());
				// long nb2 = atol(timestring.c_str());
				// long ecart = nb2 - (nb/1000);
				// std::string ecartstr ;
				// std::stringstream ss2;
				// ss2 << ecart;
				// ss2 >>ecartstr;

				// std::cout  << "ecart: " << ecart << ", pama[0]: " <<parameters[0] << ", param[o].atoi: " << nb << ", timestring.atoi :" << nb2 << ", ecartstr :" << ecartstr << std::endl;
				// message = this->getClient().getPrefixe() + "  PONG :" + parameters[0] + " " + ecartstr + " \r\n";
	message = this->getClient().getPrefixe() + "  PONG :" + parameters[0] + " \r\n";
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