#include "Command.hpp"

// std::vector<std::string> ftsplit(std::string str, std::string delimiter)
// {
// 	std::vector<std::string> values = std::vector<std::string>();

// 	size_t position;
// 	while ((position = str.find(delimiter)) != std::string::npos)
// 	{
// 		values.push_back(str.substr(0, position));
// 		str.erase(0, position + delimiter.length());
// 	}
// 	values.push_back(str);

// 	return values;
// }

Command::Command(Client *client, Server *server, std::string message)
	: client(client), server(server), query(message)
{
	std::string delimiter(":");
	size_t position;
	if ((position = message.find(delimiter)) != std::string::npos)
	{
		std::string tmp = message.substr(0, position);
		message.erase(0, position + delimiter.length());
		trailer = message;
		message = tmp;
	}

	parameters = ftsplit(message, " ");
	prefix = *(parameters.begin());
	parameters.erase(parameters.begin());

	for (size_t index = 0; index < prefix.length(); ++index)
		prefix[index] = std::toupper(prefix[index]);
}

Client &Command::getClient() { return *client; }
std::vector<std::string> Command::getParameters() { return parameters; }

void welcome(int fd, std::string prefixe, std::string nickname)
{
	std::string line;
	std::ifstream input_file("banner_ganesh.txt");
	std::string welcome = prefixe + "001 " + nickname + " " + RPL_WELCOME(prefixe);
    std::string hostinfo = prefixe + "002 " + nickname + " " + RPL_YOURHOST((std::string)SERVER_NAME, VERSION);
    std::string myinfo = prefixe + "003 " + nickname + " " + RPL_MYINFO((std::string)SERVER_NAME, VERSION, USER_MODE, CHAN_MODE);
    std::string creation = prefixe + "004 " + nickname + " " + RPL_CREATED((std::string)CREATION);
    std::cout << "parse welcome" << std::endl;
    send(fd, welcome.c_str(), welcome.size(), 0);
    send(fd, hostinfo.c_str(), hostinfo.size(), 0);
    send(fd, myinfo.c_str(), myinfo.size(), 0);
    send(fd, creation.c_str(), creation.size(), 0);
	if (!input_file.is_open())
        return ;
	while (getline(input_file, line))
	{
		send(fd, (prefixe + " 372 " + nickname + " :" + line + "\n").c_str(), (prefixe + " 372 " + nickname + " :" + line + "\n").size(), 0);
	}
}
