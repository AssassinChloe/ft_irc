#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "ft_irc.hpp"
# include "Server.hpp"

class Client 
{
    private:
        std::string username;
        std::string nickname;
        std::string status; // normal ou operator par exemple
        std::string hostaddr;
		std::string hostname;
		std::string realname;
        time_t last_ping;
        struct pollfd fd;
        // int fd;
        void write(std::string message);

        
    public:
        // lien avec poll
        // liste des channel
        Client(/* args */);
        ~Client();
        void setNickname(std::string new_nick);
        void setStatus(std::string new_status);
        std::string get_statut();
        struct pollfd getFd();
		void setUsername(std::string username);
		void setRealname(std::string realname);
        void setLastPing(time_t lastping);

        std::string getStatus();
		std::string getHostaddr();
		std::string getHostname();
		std::string getNickname();
		std::string getUsername();
		std::string getRealname();

        // void Client::sendTo(Client &toClient, std::string message);

};

#endif
