#include "Command.hpp"

#define NICK 1
#define JOIN 2
#define USER 3
#define OPER 4
#define QUIT 5
#define CAP 6
#define PING 7
#define PONG 8
#define MODE 9
#define TOPIC 10
#define LIST 11
#define KICK 12
#define PRIVMSG 13
#define KILL 14
#define PASS 15
#define PART 16

Command::Command(Client &client, Server *ganesh, std::string line)
	: client(&client), server(ganesh), query(line)
{
	std::string delimiter(":");
	size_t position;
	if ((position = line.find(delimiter)) != std::string::npos)
	{
		std::string tmp = line.substr(0, position);
		line.erase(0, position + delimiter.length());
		argLine = line;
		line = tmp;
	}

	parameters = ftsplit(line, " ");
	cmdType = *(parameters.begin());
	parameters.erase(parameters.begin());


	for (size_t x = 0; x < cmdType.length(); ++x)
		cmdType[x] = std::toupper(cmdType[x]);
}

Client &Command::getClient() { return *client; }

std::vector<std::string> Command::getParameters() { return parameters; }


int	get_cmd_id(const std::string s)
{
	if (s == "NICK")			return NICK;
	else if (s == "JOIN")		return JOIN;
	else if (s == "USER")		return USER;
	else if (s == "OPER")		return OPER;
	else if (s == "QUIT")		return QUIT;
	else if (s == "CAP")		return CAP;
	else if (s == "PING")		return PING;
	else if (s == "PONG")		return PONG;
	else if (s == "MODE")		return MODE;
	else if (s == "TOPIC")		return TOPIC;
	else if (s == "LIST")		return LIST;
	else if (s == "KICK")		return KICK;
	else if (s == "PRIVMSG")	return PRIVMSG;
	else if (s == "KILL")		return KILL;
	else if (s == "PASS")		return PASS;
	else if (s == "PART")		return PART;
	return 0;
}

void Command::execCommand()
{

	
	switch (get_cmd_id(cmdType)){
		case PING: // attendre un pong
			std::cout << "case ping" << std::endl;
			Command::Ping();
			break;
		case NICK: // changer nickname
			this->nick();
			break;
		case JOIN: // rejoindre un channel
			// if client status == welcome
			std::cout << "case join" << std::endl;
			Command::Join();
			break;
		case PART: // sortir d'un channel
			std::cout << "case part" << std::endl;
		case USER:
			this->user();
			break;	
		case OPER: 
			std::cout << "case oper" << std::endl;
			break;
		case QUIT:
			std::cout << "case quit" << std::endl;
			break;
		case CAP: // pas a gerer 
			std::cout << "case cap" << std::endl;
			break;
		case PONG:
			std::cout << "case pong" << std::endl;
			// Command::Pong();
			break;
		case MODE:
			std::cout << "case mode" << std::endl;
			break;
		case TOPIC:
			std::cout << "case topic" << std::endl;
			break;
		case LIST:
			std::cout << "case list" << std::endl;
			break;
		case KICK:
			std::cout << "case kick" << std::endl;
			break;
		case PRIVMSG:
			std::cout << "case PRIVMSG" << std::endl;
			break;
		case PASS:
			this->pass();
			break;
		case 0:
			std::cout << "commande non reconnue" << std::endl;
	}

}


