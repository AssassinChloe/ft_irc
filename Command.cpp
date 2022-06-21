#include "Command.hpp"
#include <iostream>
#include <cstring>


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

Command::Command(Client &client, std::string line)
	: client(&client), query(line)
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
	return 0;
}

void Command::execCommand()
{

	
	switch (get_cmd_id(cmdType)){
		case PING:
			std::cout << "case ping" << std::endl;
			break;
		case NICK:
			std::cout << "case nick" << std::endl;
			break;
		case JOIN:
			std::cout << "case join" << std::endl;
			break;
		case USER:
			std::cout << "case user" << std::endl;
			break;	
		case OPER:
			std::cout << "case oper" << std::endl;
			break;
		case QUIT:
			std::cout << "case quit" << std::endl;
			break;
		case CAP:
			std::cout << "case cap" << std::endl;
			break;
		case PONG:
			std::cout << "case pong" << std::endl;
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
			std::cout << "case PRIVNSG" << std::endl;
			break;
		case 0:
			std::cout << "commande non reconnue" << std::endl;
	}

}

void welcome(int fd, std::string prefixe, std::string nickname)
{
	std::string line;
	std::ifstream input_file("banner_ganesh.txt");
    send(fd, RPL_WELCOME(nickname, prefixe).c_str(), RPL_WELCOME(nickname, prefixe).size(), 0);
    send(fd, RPL_YOURHOST(prefixe, nickname, SERVER_NAME, VERSION).c_str(), RPL_YOURHOST(prefixe, nickname, SERVER_NAME, VERSION).size(), 0);
    send(fd, RPL_MYINFO(prefixe, nickname, SERVER_NAME, VERSION, USER_MODE, CHAN_MODE).c_str(), RPL_MYINFO(prefixe, nickname, SERVER_NAME, VERSION, USER_MODE, CHAN_MODE).size(), 0);
    send(fd, RPL_CREATED(prefixe, nickname, CREATION).c_str(), RPL_CREATED(prefixe, nickname, CREATION).size(), 0);
	if (!input_file.is_open())
        return ;
	//eventuellement mettre ca dans une commande MOTD (RPL 375 372 376) pour afficher le message du jour avec la commande /MOTD	
	while (getline(input_file, line))
	{
		send(fd, (prefixe + " 372 " + nickname + " :" + line + "\n").c_str(), (prefixe + " 372 " + nickname + " :" + line + "\n").size(), 0);
	}
}
