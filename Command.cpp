#include "Command.hpp"

Command::Command(Client &client, Server *ganesh, std::string line)
	: client(&client), server(ganesh)
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

int Command::checkRegistration()
{
	if (this->client->getStatus() != UNREGISTERED)
		return (0);
	return (-1);

}
int	get_cmd_id(const std::string s)
{
	if (s == "NICK")			return NICK;
	else if (s == "JOIN")		return JOIN;
	else if (s == "USER")		return USER;
	else if (s == "OPER")		return OPER;
	else if (s == "QUIT")		return QUIT;
	else if (s == "CAP")		return CAP;
	else if (s == "PING")		return PING;
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
	else if (s == "REHASH")   	return REHASH;
	else if (s == "RESTART")   	return RESTART;
	return 0;
}

void Command::execCommand()
{

	
	switch (get_cmd_id(cmdType)){
		case PING:
			this->Ping();
			break;
		case NICK:
			this->nick();
			break;
		case JOIN:
			this->Join();
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
		case CAP:
			break;
		case MODE:
			this->Mode();
			break;
		case TOPIC:
			this->Topic();
			break;
		case LIST:
			this->List();
			break;
		case KICK:
			this->Kick();
			break;
		case PRIVMSG:
			this->Privmsg();
			break;
		case PASS:
			this->pass();
			break;
		case WELCOME:
			this->welcome();
			break;
		case WHO:
			this->Who();
			break;
		case NOTICE:
			this->Notice();
			break;	
		case KILL:
			this->Kill();
			break;
		case INVITE:
			this->Invite();
			break;
		case REHASH:
			this->Rehash();
			break;
		case RESTART:
			this->Restart();
			break;
		case 0:
			send_message(*this->client, ERR_UNKNOWNCOMMAND(cmdType));
	}

}

void	send_message(Client &cl, std::string message)
{ 
	int ret = send(cl.getFd(), message.c_str(), message.size(), MSG_NOSIGNAL);
	int size = message.size();
	std::cout << "size " << size << " ret " << ret << std::endl;
	while (ret < size)
	{
		message.substr(ret, message.size());
		size = message.size();
		ret = send(cl.getFd(), message.c_str(), size, MSG_NOSIGNAL);
		std::cout << "size " << size << " ret " << ret << std::endl;

	}
	std::cout << "message envoye a " << cl.getNickname() << ": " << message << std::endl; 
}