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
    socklen_t size = sizeof (distaddr);
    
    std::vector<std::string> test;
    char buff[BUFFER_SIZE] = {0};

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
            Client newclient(tmp);
            for (unsigned long k = 0; k < test.size(); k++)
            {
                if (test[k] == "PASS" && test[k + 1] == this->_password)
                {
                    std::cout << "valid password" << std::endl;
                    newclient.setCheckPass(true);
                }
                if (test[k] == "NICK")
                {
                    std::cout << "set nickname" << std::endl;
                    newclient.setNickname(test[k + 1]);
                }
                if (test[k] == "USER")
                {
                    std::cout << "set username" << std::endl;
                    newclient.setUsername(test[k + 1]);
                }
            }
            _clients.insert(std::make_pair(tmp.fd, newclient));
            test.clear();
        }
        if (this->getClient(_poll_fd[i].fd).getCheckPass() == true && this->getClient(_poll_fd[i].fd).getNickname().size() > 0 && this->getClient(_poll_fd[i].fd).getUsername().size() > 0)
        {
            welcome(newfd, this->getClient(newfd).getPrefixe(), this->getClient(newfd).getNickname());
            this->getClient(newfd).setStatus("welcome");
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
    

    std::cout << "Client is sending data" << std::endl;
    int nbytes = recv(_poll_fd[i].fd, buff, sizeof(buff), 0);
    std::cout << "BUFF PARSE" << std::endl << buff << std::endl;
    ft_split(&test, buff);
    if (this->getClient(_poll_fd[i].fd).getCheckPass() == true)
    {
        for (unsigned long k = 0; k < test.size(); k++)
        {
                if (test[k] == "NICK")
                {
                    std::cout << "set nickname" << std::endl;
                    this->getClient(_poll_fd[i].fd).setNickname(test[k + 1]);
                }
                if (test[k] == "USER")
                {
                    std::cout << "set username" << std::endl;
                    this->getClient(_poll_fd[i].fd).setUsername(test[k + 1]);
                }
        }
    }
    else
    {
        for (unsigned long k = 0; k < test.size(); k++)
        {
            if (test[k] == "PASS" && test[k + 1] == this->_password)
            {
                std::cout << "valid password" << std::endl;
                this->getClient(_poll_fd[i].fd).setCheckPass(true);
            }
            if (test[k] == "NICK")
            {
                std::cout << "set nickname " << std::endl;
                this->getClient(_poll_fd[i].fd).setNickname(test[k + 1]);
            }
            if (test[k] == "USER")
            {
                std::cout << "set username " << std::endl;
                this->getClient(_poll_fd[i].fd).setUsername(test[k + 1]);
            }
        }
    }
    if (this->getClient(_poll_fd[i].fd).getCheckPass() == true && this->getClient(_poll_fd[i].fd).getNickname().size() > 0 && this->getClient(_poll_fd[i].fd).getUsername().size() > 0 && this->getClient(_poll_fd[i].fd).getStatus() == "default")
    {
        welcome(_poll_fd[i].fd, this->getClient(_poll_fd[i].fd).getPrefixe(), this->getClient(_poll_fd[i].fd).getNickname());
        this->getClient(_poll_fd[i].fd).setStatus("welcome");
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
        if (g_stop != 0)
            return ;
        else
        {
           g_stop = 3;
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

Client &Server::getClient(int fd)
{
    return((*(this->_clients.find(fd))).second);
}



