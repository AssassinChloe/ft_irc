/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassassi <cassassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 11:44:14 by cassassi          #+#    #+#             */
/*   Updated: 2022/07/25 14:42:39 by cassassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() {}

Server::Server(std::string port, std::string pass): _creation(time(0)), _port(port), _password(pass), _clients(std::map<int, Client>())
{
    memset(&_info, 0, sizeof(struct addrinfo));
    _info.ai_family = AF_INET;
    _info.ai_socktype = SOCK_STREAM;
    _info.ai_flags = AI_PASSIVE;
    _info.ai_protocol = 0;
}

Server::~Server() {}

Server::Server(const Server &src) 
{
    *this = src;
    return;    
}

Server &Server::operator=(Server const &rhs)
{
    if(this != &rhs)
    {
        this->_creation = rhs._creation;
        this->_port = rhs._port;
        this->_password = rhs._password;
        this->_info = rhs._info;
        this->_socket = rhs._socket;
        this->_poll_fd = rhs._poll_fd;                       
        this->_clients = rhs._clients;
        this->_channels = rhs._channels;
        this->_host = rhs._host;
        this->_name = rhs._name;
        this->_version = rhs._version;
        this->_oper_name = rhs._oper_name;
        this->_oper_pass = rhs._oper_pass;
        this->_bannerpath = rhs._bannerpath;
    }
    return (*this);
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
    int fd;
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
    for (unsigned long i = 0; i < this->_poll_fd.size(); i++)
    {
        if (nb_event == 0)
                break ;
        if (_poll_fd[i].revents & POLLIN)
        {
            nb_event--;
            if (_poll_fd[i].fd == this->_socket.fd)
            {
                fd = accept_client(i);
                checkReg(fd);
            }
            else
            {
                int fd = _poll_fd[i].fd;
                handle_client_request(i, this->getClient(fd));
                checkReg(fd);
            }
            
        }
    }
}

void Server::checkReg(int fd)
{
    if (fd < 0 || this->_clients.size() == 0 || this->_clients.find(fd) == this->_clients.end())
        return ;
    else if (this->getClient(fd).getStatus() == UNREGISTERED && this->getClient(fd).getCheckPass() == true
        && this->getClient(fd).getNickname().size() > 0 && this->getClient(fd).getUsername().size() > 0)
    {
        Command command_line(this->getClient(fd), this, "WELCOME");
        command_line.execCommand();
        this->getClient(fd).setStatus(GANESH_FRIEND);
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
            Client newclient(tmp, this);
            ret = handle_client_request((_poll_fd.size() - 1), newclient) < 0;
            if (ret < 0)
                return (-1);
            else if (ret == 0)
                _clients.insert(std::make_pair(tmp.fd, newclient));

            std::cout << _clients.size() << std::endl;
            return (newfd);
        }
    }
    return (-1);
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
    // std::cout << "buff " << buff << std::endl;
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
    lines = ftsplit(client->getBuffer(), "\n");
    for (unsigned long k = 0; k < lines.size(); k++)
    {
        Command command_line(*client, this, lines[k]);
        command_line.execCommand();
    }
    lines.clear();
}

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

Client & Server::getClient(int fd) { return((*(this->_clients.find(fd))).second); }

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
    Channel NewChan(chanName);
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

Channel &Server::getChannel(int i) { return((_channels[i])); }


int Server::getChannelNb() { return (_channels.size()); }

int Server::getChannelIndex(std::string chanName)
{
    int nb = getChannelNb();
    for (int i = 0; i < nb; i++)
    {
        if (_channels[i].getCName() == chanName)
            return (i);
    }
    return (-1);
}

std::string Server::getChannelName(int index) { return (_channels[index].getCName()); }

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
