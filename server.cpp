/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassassi <cassassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 16:39:38 by cassassi          #+#    #+#             */
/*   Updated: 2022/06/14 15:00:05 by cassassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

int stop = 0;

//je suis pas sure que exit soit autorise, je suis perdue avec ces histoire de fonction libC autorisees
void ft_error(std::string str)
{
    std::cout << str << std::endl;
    exit(1);
}

void sighandler(int sig)
{ 
    if (sig == SIGINT)
        stop = 1; 
    if (sig == SIGPIPE)
        stop = 2;
}

void *get_in_addr(struct sockaddr *sa)
{
    return &(((struct sockaddr_in*)sa)->sin_addr);
}

void add_fd(int fd, std::vector<struct pollfd> *fds)
{
    struct pollfd tmp;
    tmp.fd = fd;
    tmp.events = POLLIN;
    fds->push_back(tmp);
}

void delete_one(std::vector<struct pollfd> *fds, struct pollfd to_erase)
{
    for(std::vector<struct pollfd>::iterator it = fds->begin(); it != fds->end(); it++)
    {
        if ((*it).fd == to_erase.fd)
        {
            fds->erase(it);
            return ;
        }
    }
}

int get_listener_socket(char *port)
{
    int listener;     
    int yes=1;        
    int ret;

    struct addrinfo hints;
    struct addrinfo *res;
    struct addrinfo *tmp;
  
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
          
    if ((ret = getaddrinfo(NULL, port, &hints, &res)) != 0)
    {
        std::cout << ret << std::endl;
        perror("get addr");
        ft_error("error getaddrinfo");
    }
    for (tmp = res; tmp != NULL; tmp = tmp->ai_next) 
    {
        listener = socket(AF_INET, SOCK_STREAM, tmp->ai_protocol);
        if (listener < 0)
            continue;
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        if (bind(listener, tmp->ai_addr, tmp->ai_addrlen) < 0)
        {
            close(listener);
            continue;
        }
        break;
    }
    freeaddrinfo(res); 
    if (tmp == NULL)
        return -1;
    if (listen(listener, BACKLOG) == -1)
        return -1;
    return listener;
    
}

int main(int argc, char **argv)
{
    int sockfd, newfd;
    struct sockaddr distaddr;
    socklen_t size;
    char buff[BUFFER_SIZE] = {0}; 
    std::vector<struct pollfd> fds; // vector qui va stocker tous nos socket au fil des connexions des clients
    int nb_event;
    
    if (argc != 3)
	{
		std::cout << "./ircserv <port> <password>" << std::endl;
		return 1;
	}
	signal(SIGINT, sighandler);
    signal(SIGPIPE, sighandler); // attention pour l'instant je ne sais pas du tout ce qu'il faut faire en cas de SIGPIPE

      
    sockfd = get_listener_socket(argv[1]); //initialisation du socket d'ecoute sur le port passe en parametre
    if (sockfd < 0)
        ft_error("error getting a listening socket");
    add_fd(sockfd, &fds);
    
    //boucle server    
    while (stop == 0)
    {   
        nb_event = poll(&fds[0], fds.size(), -1);
        if (nb_event < 0)
        {
            if (stop != 0)
                break ;
            else
                ft_error("error poll");
        }
        for (unsigned long i = 0; i < fds.size(); i++) 
        {
            if (fds[i].revents & POLLIN) 
            {
                if (fds[i].fd == sockfd) 
                {
                    size = sizeof(distaddr);
                    newfd = accept(sockfd, &distaddr, &size);
                    if (newfd == -1)
                        ft_error("accept");
                    else
                        add_fd(newfd, &fds);
                }
                else if (fds.size() >= 1)// est ce que avant d'envoyer il faut check si POLLOUT est up ?
                {
                    int nbytes = recv(fds[i].fd, buff, sizeof(buff), 0);
                    int sender_fd = fds[i].fd;
                    if (nbytes <= 0) 
                    {
                        if (nbytes == 0)
                            printf("pollserver: socket %d hung up\n", sender_fd);
                        else
                            perror("recv");
                        close(fds[i].fd);
                        delete_one(&fds, fds[i]);
                    } 
                    else 
                    {
                        for(unsigned long j = 0; j < fds.size(); j++)
                        {
                            int dest_fd = fds[j].fd;
                            if (dest_fd != sockfd && dest_fd != sender_fd)
                            {
                                if (send(dest_fd, buff, nbytes, 0) == -1) 
                                    perror("send");
                            }
                        }
                    }
                }
            }
        }
    }
    close(sockfd);
    std::cout << "end" << std::endl;
}