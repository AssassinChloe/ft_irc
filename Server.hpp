#ifndef SERVER_HPP
# define SERVER_HPP
# include "ft_irc.hpp"


class server
{
    private:
        /* data */
    public:
        server(/* args */);
        ~server();

        void init();
        void run();
        void accept_client();
};

#endif