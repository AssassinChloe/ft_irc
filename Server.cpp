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

    if (init_config(CONFIG_FILE) == -1)
    {
        std::cerr << "error while reading config file" << std::endl;
        return (-1);
    }
          
    if ((ret = getaddrinfo(NULL, this->_port.c_str(), &(this->_info), &res)) != 0)
    {
        std::cerr << "Error getaddrinfo" << std::endl;
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
    fcntl(this->_socket.fd, F_SETFL, O_NONBLOCK);
    this->_socket.events = POLLIN;
    _poll_fd.push_back(this->_socket);
    return (0);
}

void Server::run()
{
    unsigned int connexions;
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
    connexions = _poll_fd.size();
    for (unsigned long i = 0; i < connexions; i++)
    {
        if (nb_event == 0)
                break ;
        if (_poll_fd[i].revents & POLLIN)
        {
            nb_event--;
            if (_poll_fd[i].fd == this->_socket.fd)
            {
                accept_client(i);
            }
            else
            {
                handle_client_request(i, this->getClient(_poll_fd[i].fd));
            }
            connexions = _poll_fd.size();
        }
    }
}

int Server::accept_client(int i)
{
    int newfd;
    struct sockaddr distaddr;
    socklen_t size = sizeof(distaddr);
    int ret;

    if (_poll_fd[i].fd == this->_socket.fd) 
    {
        std::cout << "plop accept" << std::endl;
        newfd = accept(this->_socket.fd, &distaddr, &size);
        std::cout << "plop accept" << std::endl;
        if (newfd == -1)
        {
            std::cerr << "Error accept" << std::endl;
            return (-1);
        }
        else
        {

            //test : on arrive a recuperer l'addresse ip, gethostbyname arrive pas a retrouver qu'il s'agit de
            //localhost (la ligne est presente dans /etc/hosts et devrait etre accedee) quid de ce que ca renvoit 
            //depuis un autre utilisateur mais on peut toujour faire un 
            //if (ip = 127.0.0.1) (ou avoir une liste d'IP "safe")
            //     host = localhost;
            //else
            //      host = autre;
            // et on donne pas les droit OPER et che pas quoi a autre --> mais need deux ordi pour test ce qu'on a comme info
            // struct sockaddr_in *plop = (struct sockaddr_in *)&distaddr; //changer plop et le cast a l'arrache
            // std::cout << "IP " << inet_ntoa(plop->sin_addr) << std::endl;
            // struct hostent *test = gethostbyname(inet_ntoa(plop->sin_addr));

            // std::cout << "name " << test->h_name << std::endl;
            // for (int i = 0; test->h_aliases[i] != NULL; i++)
            // {
            //     std::cout << "alias " << i << " " << test->h_aliases[i] << std::endl;
            // }
            // for (int i = 0; test->h_addr_list[i] != NULL; i++)
            // {
            //     std::cout << "addr " << i << " " << test->h_addr_list[i] << std::endl;
            // }
            //end test

            struct pollfd tmp;
            tmp.fd = newfd;
            tmp.events = POLLIN;
            _poll_fd.push_back(tmp);
            Client newclient(tmp, this);
            ret = handle_client_request((_poll_fd.size() - 1), newclient) < 0;
            if (ret < 0)
                return (-1);
            else if (ret == 0)
                _clients.insert(std::make_pair(tmp.fd, newclient));
        }
    }
    return (0);
}

int Server::handle_client_request(int i, Client &client)
{
    char buff[BUFFER_SIZE] = {0};
    int nbytes;
    size_t position;
    
    nbytes = recv(_poll_fd[i].fd, buff, sizeof(buff), 0);
    if (nbytes <= 0)
        return (this->retRecv(_poll_fd[i].fd, nbytes));
    client.addBuffer(buff);
    std::cout << "buff " << buff << std::endl;
    if ((position = client.getBuffer().find("\n")) != std::string::npos)
    {
        this->dispatch(&client);
        return (0);
    }
    return (1);
}

void Server::dispatch(Client *client)
{
    std::vector<std::string> lines;
    
    lines = ftsplit(client->getBuffer(), "\n"); // "\r\n" ou \n ? pour regler le probleme de reconnection par hexchat ?
    for (unsigned long k = 0; k < lines.size(); k++)
    {
        Command command_line(*client, this, lines[k]);
        command_line.execCommand();
    }
    lines.clear();
    if (client->getStatus() == UNREGISTERED && client->getCheckPass() == true
        && client->getNickname().size() > 0 && client->getUsername().size() > 0)
    {
        Command command_line(*client, this, "WELCOME");
        command_line.execCommand();
        client->setStatus(GANESH_FRIEND);
    } 
}

// int Server::reception_concatenation(int i)
// {
//     char buff[BUFFER_SIZE] = {0};
//     int nbytes;
//     size_t position;

//     nbytes = recv(_poll_fd[i].fd, buff, sizeof(buff), 0);
//     *buffer = static_cast<std::string>(buff);
//     std::cout << "buffer: " << buff << " : " << nbytes << std::endl;
//     if (nbytes <= 0)
//         return (this->retRecv(_poll_fd[i].fd, nbytes));
//     else
//     {
//         while ((position = buffer->find("\n")) == std::string::npos)
//         {
//             nbytes = recv(_poll_fd[i].fd, buff, sizeof(buff), 0);
//             if (nbytes <= 0)
//                 return (this->retRecv(_poll_fd[i].fd, nbytes));
//             *buffer += static_cast<std::string>(buff);
//             std::cout << "buff: " << buff << " buffer: " << buffer <<std::endl;

//         } 
//     }
//     return (0);
// }

void Server::deleteClient(int fd)
{
    if (this->getClient(fd).getChanList().size() > 0)
    {
        Command command_line(this->getClient(fd), this, "JOIN 0");
        command_line.execCommand();
    }
    close(fd);
    if (_clients.size() > 0)
    {
        _clients.erase(fd);
    }
    for (std::vector<struct pollfd>::iterator it = this->_poll_fd.begin(); it != this->_poll_fd.end(); it++)
    {
        if ((*it).fd == fd)
        {
            _poll_fd.erase(it);
            return ;
        }
    }
}


Client & Server::getClient(int fd)
{
    return((*(this->_clients.find(fd))).second);
}

Client & Server::getClient(std::string nickname)
{
    std::map<int, Client>::iterator it;
    for (it = this->_clients.begin(); it != this->_clients.end(); it++)
    {
        if ((*it).second.getNickname() == nickname)
            break;
    }
    return ((*it).second);
}


std::map<int, Client> & Server::getClientList()
{
    return (this->_clients);
}

std::string Server::getPass() const
{
    return (this->_password);
}



int Server::retRecv(int fd, int nbytes)
{
    if (nbytes == 0)
        this->deleteClient(fd);
    else
    {
        this->deleteClient(fd);
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
    Channel NewChan(chanName);//(this, chanName);
    if (NewChan.getCName() != "")
    {
        _channels.push_back(NewChan);
    }

}

void Server::delChannel(std::string chanName)
{
    for(std::vector<Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
    {
        if ((*it).getCName() == chanName)
        {
            this->_channels.erase(it);
            return ;
        }
    }
}


Channel &Server::getChannel(int i)
{
    return((_channels[i]));
}


int Server::getChannelNb()
{
    return (_channels.size());
}

int Server::getChannelIndex(std::string chanName)
{
    int nb = getChannelNb();
    for (int i = 0; i < nb; i++)
    {
        if (_channels[i].getCName() == chanName)
            return (i);
    }
    return (-1); // 
}

Channel &Server::getChannel(std::string chanName)
{
    int nb = getChannelNb();
    for (int i = 0; i < nb; i++)
    {
        if (_channels[i].getCName() == chanName)
            return (_channels[i]);
    }
    return (_channels[0]); // attention a modifier !!! Ne me semble etre utilise que dans fonction quit -> a modifier ou s'assurer qu'iln'y a pas d'erreur possible
}


std::string Server::getChannelName(int index)
{
    return (_channels[index].getCName());
}

std::vector<struct pollfd> Server::getPollFdList()
{
    return (this->_poll_fd);
}

int Server::getSocketFd() { return _socket.fd;}

void Server::cleanClose() 
{
    close(_socket.fd); 
    std::vector<int> FDs;
    int i = 0;
    for (std::map<int, Client>::iterator it = getClientList().begin(); it != getClientList().end(); it++)
            {
                FDs.push_back(it->first);
                i++;
            }

    for (int j = 0; j<i; j++)
    {
        deleteClient(FDs[j]);
    }
}
