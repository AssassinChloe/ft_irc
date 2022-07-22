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

std::string check_params(std::string param)
{
	if (param.size() != 0)
		return (param);
	return("default");
}

int check_if_channel(std::string name)
{
	size_t position;
	std::string charset = CHANNEL_VALID_CHAR;
	if ((position = charset.find(name[0])) == std::string::npos)
		return (0);
	return (1);
}

int searchIfMode(std::string tofind, std::string target)
{
    size_t position;
    for (size_t i = 0; i != tofind.size(); i++)
    {
        if ((position = target.find(tofind[i])) != std::string::npos)
            return (1);
    }
    return (0);
}

int searchIfMode(char tofind, std::string target)
{
    size_t position;
    if ((position = target.find(tofind)) != std::string::npos)
            return (1);
    return (0);
}

std::string lowercase(std::string s)
{
	int nb = s.size();
	std::string ret;
	for(int i = 0; i < nb; i++)
		ret += tolower(s[i]);
	std::cout << "string init : " << s << ", string lower : " << ret << std::endl;
	return ret;
}