/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassassi <cassassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 16:56:20 by cassassi          #+#    #+#             */
/*   Updated: 2022/07/19 13:20:29 by cassassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "ft_irc.hpp"

class Server;

class Client 
{
    private:
        
        
        Server *server;
        std::string     username;
        std::string     nickname;
        std::string     status; 
        std::string     hostaddr;
		std::string     hostname;
        bool            pass_checked;
        time_t          last_ping;
        std::string     realname;
        struct pollfd   fd;
        std::string     buffer;
        std::map<std::string, std::string> channel_list;
        Client();

        
    public:
        Client(struct pollfd newfd, Server *ganesh);
        ~Client();

		void setUsername(std::string username);
        void setNickname(std::string new_nick);
        void setRealname(std::string realname);
        void setStatus(std::string new_status);
        void setLastPing(time_t lastping);
        void setCheckPass(bool checked);
        void setBuffer(std::string str);

        std::string getBuffer();
		std::string getUsername();
		std::string getNickname();
		std::string getRealname();
        std::string getStatus();
		std::string getHostaddr();
		std::string getHostname();
        bool getCheckPass();
        int getFd();

        std::string getPrefixe();

        std::map<std::string, std::string> getChanList();
        std::string getChanMode(std::string name);
        void addBuffer(char *str);
        void addChannel(std::string name, std::string mode);
        void delChannel(std::string name);
        
        void setMode(std::string chan, std::string mode);
        int addMode(std::string chan, char mode);
		int delMode(std::string chan, char mode);
};

#endif
