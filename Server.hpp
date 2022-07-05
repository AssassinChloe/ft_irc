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

    public:
        Server(std::string port, std::string pass);
        ~Server();

        int init();
        void run();
        int using_poll();
        int handle_client_request(int i, Client &client);
        int accept_client(int i);
        void deleteClient(int fd);
        int reception_concatenation(int fd, std::string *buffer);
        int retRecv(int i, int nbytes);
        void dispatch(Client &client, std::string buff);

        Client &getClient(int fd);
        Client &getClient(std::string name);
        std::map<int, Client> &getClientList();
        std::string getPass() const;
        std::string getCreation() const;
        // int getNbChannel(); // voir si on veut un nb max de channels
        void addChannel(std::string chanName);
        void delChannel(std::string chanName);
        Channel &getChannel(int i);
        Channel &getChannel(std::string chanName);
        std::string getChannelName(int index);
        int getChannelIndex(std::string chanName);
        int getChannelNb();
        std::vector<struct pollfd> getPollFdList();

};

#endif

