/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassassi <cassassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 16:56:20 by cassassi          #+#    #+#             */
/*   Updated: 2022/06/30 14:05:56 by cassassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "ft_irc.hpp"

class Client 
{
    private:
        
        std::string     username;
        std::string     nickname;
        std::string     status; // normal ou operator par exemple
        std::string     hostaddr;
		std::string     hostname;
        bool            pass_checked;
        time_t          last_ping;
        struct pollfd   fd;
        std::map<std::string, std::string> channel_list;

        Client();
        void write(std::string message);

        
    public:
        // lien avec poll
        // liste des channel
        Client(struct pollfd newfd);
        ~Client();

		void setUsername(std::string username);
        void setNickname(std::string new_nick);
        void setStatus(std::string new_status);
        void setLastPing(time_t lastping);
        void setCheckPass(bool checked);

		std::string getUsername();
		std::string getNickname();
        std::string getStatus();
		std::string getHostaddr();
		std::string getHostname();
        bool getCheckPass();
        int getFd();
        std::map<std::string, std::string> getChanList();

        // void Client::sendTo(Client &toClient, std::string message);
        std::string getPrefixe();

        void addChannel(std::string name, std::string mode);
        void delChannel(std::string name);
};

#endif
