/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassassi <cassassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 16:55:12 by cassassi          #+#    #+#             */
/*   Updated: 2022/06/23 14:21:55 by cassassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# include "ft_irc.hpp"

class Client;

class Channel;

class Server
{
    private:
        std::string                 _port;
        std::string                 _password;
        struct addrinfo             _info;
        struct pollfd               _socket;
        std::vector<struct pollfd>  _poll_fd;             
        std::map<int, Client>       _clients;
        std::vector<class Channel>        _channels;

    public:
        Server(std::string port, std::string pass);
        ~Server();

        int init();
        void run();
        int using_poll();
        int handle_client_request(int i);
        int accept_client(int i);
        void deleteClient(int fd);
        void dispatch(Client &client, char *buff);
        int retRecv(int i, int nbytes);
        Client &getClient(int fd);
        std::map<int, Client> &getClientList();
        std::string getPass() const;
        // int getNbChannel(); // voir si on veut un nb max de channels
        void addChannel(std::string chanName);
        Channel &getChannel(int i);
        int getChannelSize();
        std::string getChannelName(int index);
};

#endif

