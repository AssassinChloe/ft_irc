#include "Server.hpp"
#include "ft_irc.hpp"

Server::Server(std::string port, std::string pass): _port(port), _password(pass)
{
    memset(&_info, 0, sizeof(struct addrinfo));
    _info.ai_family = AF_INET;
    _info.ai_socktype = SOCK_STREAM;
    _info.ai_flags = AI_PASSIVE;
    _info.ai_protocol = 0;
}

Server::~Server()
{
}

int Server::init()
{
    int opt = 1;        
    int ret;
    struct addrinfo *res;
    struct addrinfo *tmp;
          
    if ((ret = getaddrinfo(NULL, this->_port.c_str(), &(this->_info), &res)) != 0)
    {
        std::cerr << "Error getaddinfo" << std::endl;
        return (-1);
    }
    for (tmp = res; tmp != NULL; tmp = tmp->ai_next) 
    {
        this->_socket.fd = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);
        if (this->_socket.fd < 0)
            continue;
        setsockopt(this->_socket.fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
        if (bind(this->_socket.fd, tmp->ai_addr, tmp->ai_addrlen) < 0) 
        {
            close(this->_socket.fd);
            continue;
        }
        break; 
    }
    freeaddrinfo(res); 
    if (tmp == NULL || this->_socket.fd < 0 || listen(this->_socket.fd, BACKLOG) == -1)
        return (-1); 
    this->_socket.events = POLLIN;
    _poll_fd.push_back(this->_socket);
    return (0);
}

int Server::accept_client(int i)
{
    int newfd;
    struct sockaddr distaddr;
    std::vector<std::string> test;
    char buff[BUFFER_SIZE] = {0};
    std::string your_host = RPL_YOURHOST((std::string)SERVER_NAME, VERSION);
    std::string my_info = RPL_MYINFO((std::string)SERVER_NAME, VERSION, USER_MODE, CHAN_MODE);
    std::string creation = RPL_CREATED((std::string)"15/06/2022");
    socklen_t size = sizeof (distaddr);

    if (_poll_fd[i].fd == this->_socket.fd) 
    {
        newfd = accept(this->_socket.fd, &distaddr, &size);
        if (newfd == -1)
        {
            std::cerr << "Error accept" << std::endl;
            return (-1);
        }
        else
        {
            struct pollfd tmp;
            tmp.fd = newfd;
            tmp.events = POLLIN;
            _poll_fd.push_back(tmp);
            recv(newfd, buff, sizeof(buff), 0);
            std::cout << "BUFF ACCEPT" << std::endl << buff << std::endl;
                ft_split(&test, buff);
            for (unsigned long k = 0; k < test.size(); k++)
            {
                if (test[k] == "PASS" && test[k + 1] == this->_password)
                {
                    send(newfd, RPL_WELCOME(test[k+1]).c_str(), RPL_WELCOME(test[k+1]).size(), 0);
                    std::cout << "rpl welcome sent" << std::endl;
                    send(newfd, your_host.c_str() , your_host.size(), 0);
                    std::cout << "rpl yourhost sent" << std::endl;
                    send(newfd, my_info.c_str(), my_info.size(), 0);
                    std::cout << "rpl myinfo sent" << std::endl;
                    send(newfd, creation.c_str(), creation.size(), 0);
                    std::cout << "rpl created sent" << std::endl;
                }
            }
            test.clear();
        }
    }
    return (0);
}

void Server::ft_split(std::vector<std::string> *tab, std::string str)
{
    std::istringstream iss(str);
    std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter(*tab));
}

int Server::parse_data(int i)
{
    std::vector<std::string> test;
    char buff[BUFFER_SIZE] = {0};
    std::string your_host = RPL_YOURHOST((std::string)SERVER_NAME, VERSION);
    std::string my_info = RPL_MYINFO((std::string)SERVER_NAME, VERSION, USER_MODE, CHAN_MODE);
    std::string creation = RPL_CREATED((std::string)"15/06/2022");
    std::cout << "Client is sending data" << std::endl;
    int nbytes = recv(_poll_fd[i].fd, buff, sizeof(buff), 0);
    std::cout << "BUFF PARSE" << std::endl << buff << std::endl;
    ft_split(&test, buff);
    for (unsigned long k = 0; k < test.size(); k++)
    {
        if (test[k] == "NICK")
        {
            std::cout << test[k + 1] << " is in da place" << std::endl;
        }
    }
    test.clear();
    if (nbytes <= 0)
    {
        if (nbytes == 0)
            std::cout << "pollserver: socket " << _poll_fd[i].fd << " hung up" << std::endl;
        else
            perror("recv");
        close(_poll_fd[i].fd);
        for(std::vector<struct pollfd>::iterator it = _poll_fd.begin(); it != _poll_fd.end(); it++)
        {
            if ((*it).fd == _poll_fd[i].fd)
            {
                _poll_fd.erase(it);
                break ;
            }
        }
        return (-1);
    } 
    else
    {
        for(unsigned long j = 0; j < _poll_fd.size(); j++)
        {
            int dest_fd = _poll_fd[j].fd;
            if (dest_fd != _socket.fd && dest_fd != _poll_fd[i].fd)
            {
                if (send(dest_fd, buff, nbytes, 0) == -1) 
                    return (-1);
            }
        }
    }
    return (0);
}

void Server::run()
{
    int nb_event = poll(&_poll_fd[0], _poll_fd.size(), -1);
    if (nb_event < 0)
    {
        if (stop != 0)
            return ;
        else
        {
            stop = 3;
            std::cerr << "Error poll" << std::endl;
            return ;
        }
    }
    for (unsigned long i = 0; i < _poll_fd.size(); i++)
    {
        if (nb_event == 0)
                break ;
        if (_poll_fd[i].revents & POLLIN)
        {
            nb_event--;
            if (_poll_fd[i].fd == this->_socket.fd)
                accept_client(i);
            else
               parse_data(i);
        }
    }
}



