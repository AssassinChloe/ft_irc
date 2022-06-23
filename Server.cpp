
#include "Server.hpp"

Server::Server(std::string port, std::string pass): _creation(time(0)), _port(port), _password(pass)
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
            Client newclient(tmp);
            handle_client_request((_poll_fd.size() - 1), newclient);
            _clients.insert(std::make_pair(tmp.fd, newclient));
        }
    }
    return (0);
}

int Server::handle_client_request(int i, Client &client)
{
    std::string buffer;

    if (this->reception_concatenation(i, &buffer) < 0)
        return -1;
    this->dispatch(client, buffer);
    return (0);
}

int Server::reception_concatenation(int i, std::string *buffer)
{
    char buff[BUFFER_SIZE] = {0};
    int nbytes;
    size_t position;

    nbytes = recv(_poll_fd[i].fd, buff, sizeof(buff), 0);
    *buffer = static_cast<std::string>(buff);
    if (nbytes <= 0)
        return (this->retRecv(i, nbytes));
    else
    {
        while ((position = buffer->find("\n")) == std::string::npos)
        {
            nbytes = recv(_poll_fd[i].fd, buff, sizeof(buff), 0);
            *buffer += static_cast<std::string>(buff);
        } 
    }
    return (0);
}

void Server::deleteClient(int i)
{
    std::cout << "pollserver: socket " << _poll_fd[i].fd << " hung up" << std::endl;
    close(_poll_fd[i].fd);
    _clients.erase(_poll_fd[i].fd);
    for (std::vector<struct pollfd>::iterator it = _poll_fd.begin(); it != _poll_fd.end(); it++)
    {
        if ((*it).fd == _poll_fd[i].fd)
        {
            _poll_fd.erase(it);
            return ;
        }
    }
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
                handle_client_request(i, this->getClient(_poll_fd[i].fd));
        }
    }
}

Client & Server::getClient(int fd)
{
    return((*(this->_clients.find(fd))).second);
}

std::map<int, Client> & Server::getClientList()
{
    return (this->_clients);
}

std::string Server::getPass() const
{
    return (this->_password);
}

void Server::dispatch(Client &client, std::string buff)
{
    std::vector<std::string> lines;
    
    lines = ftsplit(buff, "\r\n");
    for (unsigned long k = 0; k < lines.size(); k++)
    {
        Command command_line(client, this, lines[k]);
        command_line.execCommand();
    }
    lines.clear();
    if (client.getStatus() == "default" && client.getCheckPass() == true
        && client.getNickname().size() > 0 && client.getUsername().size() > 0)
    {
        Command command_line(client, this, "WELCOME");
        command_line.execCommand();
        client.setStatus("welcome");
    } 
}

int Server::retRecv(int i, int nbytes)
{
    if (nbytes == 0)
        this->deleteClient(i);
    else
    {
        this->deleteClient(i);
        perror("recv");
    }
    return (-1);
}

std::string Server::getCreation() const
{
    return(ctime(&this->_creation));
}

void Server::addChannel(std::string chanName)
{
    std::cout << "debut du add channel" << std::endl;
    Channel NewChan(chanName);//(this, chanName);
    if (NewChan.getCName() != "")
    {
        std::cout << "channel cree" << std::endl;
        _channels.push_back(NewChan);
        std::cout << "channel ajoute a la liste" << std::endl;
    }

// petite verif
    // int nb = getChannelSize();
    // for (int i=0; i<nb; i++)
    // std::cout << i << " : " <<_channels[i].getCName() << std::endl;
}

Channel &Server::getChannel(int i)
{
    return((_channels[i]));
}

// Channel &Server::getChannel(std::string chanName)
// {
//     int nb = getChannelSize();
//     for (int i = 0; i < nb; i++)
//     {
//         if (_channels[i].getName() == chanName)
//             return (_channels[i]);
//     }
// }

int Server::getChannelSize()
{
    return (_channels.size());
}

std::string Server::getChannelName(int index)
{
    return (_channels[index].getCName());
}