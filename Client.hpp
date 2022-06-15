#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "ft_irc.hpp"
# include "Server.hpp"

class client 
{
    private:
        std::string username;
        std::string nickname;
        std::string statut;

        
    public:
        client(/* args */);
        ~client();
        void set_nickname(std::string new_nick);
        void set_statut(std::string new_statut);
        std::string get_statut();

};

#endif
