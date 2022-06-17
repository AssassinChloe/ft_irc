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