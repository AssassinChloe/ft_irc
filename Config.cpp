/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmercier <vmercier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 11:41:51 by vmercier          #+#    #+#             */
/*   Updated: 2022/07/25 11:41:54 by vmercier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"


int Server::init_config(std::string configpath)
{
	std::ifstream ifs(configpath.c_str(), std::ifstream::in);
	if (!ifs.good())
    {
        std::cout << "error while reading config file" << std::endl;
        return -1;
    }

    _host = "localhost";
    _name = "Ganesh's_Daugthers";
    _version = "0.1";
    _oper_name = "Ganesh_Army";
    _oper_pass = "securisedpassword";
    _bannerpath = "banner_ganesh.txt";

	std::string delimiter = "=";
	size_t position;
	std::string line;
	while (!ifs.eof())
	{
        line = "";
		std::getline(ifs, line);
		if ((position = line.find('=')) == std::string::npos)
			continue;
		std::string key = line.substr(0, position);
		line.erase(0, position + delimiter.length());

		if (key == "HOST" && line.size() != 0)
            _host = line;
        else if (key == "SERVER_NAME" && line.size() != 0)
            _name = line;
        else if (key == "VERSION" && line.size() != 0)
            _version = line;
        else if (key == "OPER_NAME" && line.size() != 0)
            _oper_name = line;
        else if (key == "OPER_PASS" && line.size() != 0)
            _oper_pass = line;
        else if (key == "BANNER_PATH" && line.size() != 0)
            _bannerpath = line;

	}
	ifs.close();
    return 0;
}

const char *Server::getBanner() { return _bannerpath.c_str(); }
std::string Server::getHost() { return _host; }
std::string Server::getName(){ return _name; }
std::string Server::getVersion() { return _version; }
std::string Server::getOperName() { return _oper_name; }
std::string Server::getOperPass() { return _oper_pass; }
