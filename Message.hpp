#ifndef MESSAGE_HPP // conseille par ircgod... a voir
# define MESSAGE_HPP

# include "ft_irc.hpp"
# include "Client.hpp"
# include "Server.hpp"

class message 
{
    private:
        /* data */
    public:

        client sender;
        client receiver;
        std::string command_call;
        message(/* args */);
        ~message();

};

#endif