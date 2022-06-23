
#include "ft_irc.hpp"

std::vector<std::string>		ftsplit(std::string s, std::string splitter)
{
	std::vector<std::string>	now;
	std::string					tmp;
	size_t						ret = 0;
	size_t						k = 0;
	// size_t						i;

	ret = 0;
	if (splitter.empty())
	{
		now.push_back("");
		return (now);
	}
	while (ret < s.length())
	{
		k = ret;
		ret = s.find(splitter, ret);
		if (k != ret)
		{
			if (ret == std::string::npos)
				now.push_back(s.substr(k));
			else
			{
				now.push_back(s.substr(k));
				now.back().resize(ret - k);
			}
		}
		if (ret == std::string::npos)
			break;
		ret += splitter.length();
	}
	// i = now.size();
	// for (size_t j=i; j>=0; j--)
	// {
	// 	tmp = now[j];
	// 	now[j] = tmp + "\0";
	// }
	return (now);
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

std::string check_params(std::string param)
{
	if (param.size() != 0)
		return (param);
	return("default");
}