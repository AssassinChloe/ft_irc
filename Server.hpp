/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassassi <cassassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 16:55:12 by cassassi          #+#    #+#             */
/*   Updated: 2022/06/21 16:55:13 by cassassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# include "ft_irc.hpp"

class Client;

class Server
{
    private:
        std::string                 _port;
        std::string                 _password;
        struct addrinfo             _info;
        struct pollfd               _socket;
        std::vector<struct pollfd>  _poll_fd;             
        std::map<int, Client>       _clients;

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
        std::string getPass();
};

#endif

