/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassassi <cassassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 16:39:38 by cassassi          #+#    #+#             */
/*   Updated: 2022/06/14 16:18:23 by cassassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

int stop = 0;

//je suis pas sure que exit soit autorise plus la a des fin de debug
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

// gestion du vector de struct pollfd : ADD (creer une class ?)
void add_fd(int fd, std::vector<struct pollfd> *fds)
{
    struct pollfd tmp;
    tmp.fd = fd;
    tmp.events = POLLIN; // on indique qu'on va s'interesser aux events de type POLLIN (Il y a des données en attente de lecture.)
    fds->push_back(tmp);
}

// gestion du vector de struct pollfd : ERASE (creer une class ?)
void delete_fd(std::vector<struct pollfd> *fds, struct pollfd to_erase)
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
    hints.ai_protocol = 0;
          
    if ((ret = getaddrinfo(NULL, port, &hints, &res)) != 0)
        ft_error("error getaddrinfo");
    for (tmp = res; tmp != NULL; tmp = tmp->ai_next) 
    {
        std::cout << "for loop listener" << std::endl;
        listener = socket(AF_INET, SOCK_STREAM, tmp->ai_protocol);
        if (listener < 0)
            continue; // si listener < 0, on ne va pas dans le reste de la boucle for et on passe direct a next (incrementation)
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)); // on arrive ici avec un fd valide
        if (bind(listener, tmp->ai_addr, tmp->ai_addrlen) < 0) 
        {
            close(listener); //donc si bind echoue on close le fd
            continue; //et on va tester avec le next
        }
        break; //si socket et bind ont marche on sort du for avec notre fd valide et bind 
    } // et si on passe jamais par le break on finira par sortir de la boucle for qd tmp == null
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
    std::vector<struct pollfd> fds; // vector qui va stocker tous nos sockets(fd) au fil des connexions des clients
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
        nb_event = poll(&fds[0], fds.size(), -1); //nb event == nombre de fd (mais on sait pas lequels) avec un evenement qui nous interesse (ici POLLIN)
        if (nb_event < 0)
        {
            if (stop != 0) //ici je check si poll renvoie < 0 parce qu'on a intercepte un signal ou si y a eu une erreur de poll (marche avec ctrl+c)
                break ;
            else
                ft_error("error poll");
        }
        for (unsigned long i = 0; i < fds.size(); i++) //on parcourt le vector pour trouver le FD avec un event positif
        {
            if (nb_event == 0)
                break ;
            if (fds[i].revents & POLLIN) //on rentre s'il s'agit d'un FD avec des donnees en attente de lecture (POLLIN)
            {
                nb_event--; // ici on decremente pour pouvoir quitter la boucle for des qu'on a trouve le(s) fd avec evenement au lieu de parcourir systematiquement l'entierete du vector (avec 3 fd on s'en fout, avec des miliers j'imagine que ca peut aider des fois)
                if (fds[i].fd == sockfd) // si c'est notre socket d'ecoute qui a des donnees en attente, il s'agit d'une nouvelle connexion
                {
                    size = sizeof(distaddr);
                    newfd = accept(sockfd, &distaddr, &size);
                    if (newfd == -1)
                        ft_error("accept");
                    else
                        add_fd(newfd, &fds);
                }
                else // sinon c'est que c'est un client deja connecte qui nous dit qu'on a des trucs a lire
                {
                    int nbytes = recv(fds[i].fd, buff, sizeof(buff), 0);
                    int sender_fd = fds[i].fd; //sert que pour le message de fermeture d'un fd
                    if (nbytes <= 0)
                    {
                        if (nbytes == 0) // si y a rien a lire c'est que le client s'est deconnecte 
                            printf("pollserver: socket %d hung up\n", sender_fd);
                        else
                            perror("recv");
                        close(fds[i].fd);
                        delete_fd(&fds, fds[i]); //on close et on supprime le fd bugue ou desormais invalide du tableau
                    } 
                    else // sinon on renvoie les donnees a tous les fd sauf celui d'ecoute et celui d'ou provient le message
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
//voila