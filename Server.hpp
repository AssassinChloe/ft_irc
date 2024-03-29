/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassassi <cassassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 11:44:10 by cassassi          #+#    #+#             */
/*   Updated: 2022/07/25 11:57:56 by cassassi         ###   ########.fr       */
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
        time_t                      _creation;
        std::string                 _port;
        std::string                 _password;
        struct addrinfo             _info;
        struct pollfd               _socket;
        std::vector<struct pollfd>  _poll_fd;             
        std::map<int, Client>       _clients;
        std::vector<class Channel>  _channels;
        std::string                 _host;
        std::string                 _name;
        std::string                 _version;
        std::string                 _oper_name;
        std::string                 _oper_pass;
        std::string                 _bannerpath;
        Server();
        Server(const Server &src);
		Server &operator=(Server const &rhs);

    public:
        Server(std::string port, std::string pass);
        ~Server();

        int init();
        void run();
        int handle_client_request(int i, Client &client);
        int accept_client(int i);
        void deleteClient(int fd);
        int retRecv(int i, int nbytes);
        void dispatch(Client *client);

        Client &getClient(int fd);
        Client &getClient(std::string name);
        std::map<int, Client> &getClientList();
        std::string getPass() const;
        std::string getCreation() const;
        void addChannel(std::string chanName);
        void delChannel(std::string chanName);
        Channel &getChannel(int i);
        std::string getChannelName(int index);
        int getChannelIndex(std::string chanName);
        int getChannelNb();
        int init_config(std::string configpath);
        const char *getBanner();
        std::string getHost();
        std::string getName();
        std::string getVersion();
        std::string getOperName();
        std::string getOperPass();
        void cleanClose();
        void checkReg(int fd);
};

#endif

