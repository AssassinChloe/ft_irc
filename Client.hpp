#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "ft_irc.hpp"
# include "Server.hpp"

class Client 
{
    private:
        Client();
        std::string     username;
        std::string     nickname;
        std::string     status; // normal ou operator par exemple
        std::string     hostaddr;
		std::string     hostname;
        bool            pass_checked;
        struct pollfd   fd;

        void write(std::string message);

        
    public:
        // lien avec poll
        // liste des channel
        Client(struct pollfd newfd);
        ~Client();

        void setNickname(std::string new_nick);
        void setStatus(std::string new_status);
		void setUsername(std::string username);
		void setRealname(std::string realname);
        void setCheckPass(bool checked);

        std::string get_statut();
        int getFd();
        std::string getStatus();
		std::string getHostaddr();
		std::string getHostname();
		std::string getNickname();
		std::string getUsername();
		std::string getRealname();
        bool getCheckPass();

        // void Client::sendTo(Client &toClient, std::string message);
        std::string getPrefixe();
};

#endif
