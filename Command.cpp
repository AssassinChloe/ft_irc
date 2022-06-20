#include "ft_irc.hpp"

void welcome(int fd, std::string prefixe, std::string nickname)
{
        std::string welcome = prefixe + "001 " + nickname + " " + RPL_WELCOME(prefixe);
        std::string hostinfo = prefixe + "002 " + nickname + " " + RPL_YOURHOST((std::string)SERVER_NAME, VERSION);
        std::string myinfo = prefixe + "003 " + nickname + " " + RPL_MYINFO((std::string)SERVER_NAME, VERSION, USER_MODE, CHAN_MODE);
        std::string creation = prefixe + "004 " + nickname + " " + RPL_CREATED((std::string)CREATION);
        std::cout << "parse welcome" << std::endl;
        send(fd, welcome.c_str(), welcome.size(), 0);
        send(fd, hostinfo.c_str(), hostinfo.size(), 0);
        send(fd, myinfo.c_str(), myinfo.size(), 0);
        send(fd, creation.c_str(), creation.size(), 0);
}