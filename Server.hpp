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
        int parse_data(int i);
        int accept_client(int i);

        void ft_split(std::vector<std::string> *tab, std::string str);

        Client &getClient(int fd);
};

#endif

