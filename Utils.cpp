
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