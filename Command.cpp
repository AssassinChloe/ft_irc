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
#define WELCOME 17
#define WHO 18
#define NOTICE 19
#define INVITE 20



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
Command::~Command() { }

Client &Command::getClient() { return *client; }

std::vector<std::string> Command::getParameters() { return parameters; }
std::string Command::getArgLine() { return argLine; }

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
	else if (s == "WELCOME")    return WELCOME;
	else if (s == "WHO")   		return WHO;
	else if (s == "NOTICE")   	return NOTICE;
	else if (s == "KILL")   	return KILL;
	else if (s == "INVITE")   	return INVITE;
	return 0;
}

void Command::execCommand()
{

	
	switch (get_cmd_id(cmdType)){
		case PING: // attendre un pong
			Command::Ping();
			break;
		case NICK: // changer nickname
			this->nick();
			break;
		case JOIN: // rejoindre un channel
			Command::Join();
			break;
		case PART: 
			this->part();
			break;
		case USER:
			this->user();
			break;	
		case OPER:
			this->oper(); 
			break;
		case QUIT:
			this->quit(); 
			break;
		case CAP: // pas a gerer 
			break;
		case PONG:
			// Command::Pong();
			break;
		case MODE:
			Command::Mode();
			break;
		case TOPIC:
			Command::Topic();
			break;
		case LIST:
			Command::List();
			break;
		case KICK:
			Command::Kick();
			break;
		case PRIVMSG:
			Command::Privmsg();
			break;
		case PASS:
			this->pass();
			break;
		case WELCOME:
			this->welcome();
			break;
		case WHO:
			Command::Who();
			break;
		case NOTICE:
			Command::Notice();
			break;	
		case KILL:
			Command::Kill();
			break;
		case INVITE:
			Command::Invite();
			break;
		case 0:
			send_message(*this->client, ERR_UNKNOWNCOMMAND(cmdType));
	}

}

void	send_message(Client &cl, std::string message)
{
	send(cl.getFd(), message.c_str(), message.size(), MSG_NOSIGNAL);
	std::cout << "message envoye a " << cl.getNickname() << ": " << message << std::endl; 
}