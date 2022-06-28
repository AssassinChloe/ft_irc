#ifndef MESSAGE_HPP // conseille par ircgod... a voir
# define MESSAGE_HPP

# include "ft_irc.hpp"

class message 
{
    private:
        /* data */
    public:

        Client sender;
        Client receiver;
        std::string command_call;
        message(/* args */);
        ~message();

};

#endif