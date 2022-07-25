/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassassi <cassassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 11:37:06 by cassassi          #+#    #+#             */
/*   Updated: 2022/07/25 11:37:42 by cassassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

std::vector<std::string>		ftsplit(std::string s, std::string splitter)
{
	std::vector<std::string>	now;
	size_t						ret = 0;
	size_t						k = 0;

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
	return ret;
}