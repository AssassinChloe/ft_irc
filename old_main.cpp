/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   old_main.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassassi <cassassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 16:39:38 by cassassi          #+#    #+#             */
/*   Updated: 2022/06/16 15:09:57 by cassassi         ###   ########.fr       */
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


void ft_split(std::vector<std::string> *tab, std::string str)
{
    std::istringstream iss(str);
    std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter(*tab));
}

int main(int argc, char **argv)
{
    int sockfd, newfd, nbytes;
    struct sockaddr distaddr;
    socklen_t size;
    char buff[BUFFER_SIZE] = {0}; 
    std::vector<struct pollfd> fds; // vector qui va stocker tous nos sockets(fd) au fil des connexions des clients
    std::vector<std::string> test;
    int nb_event;
    
    if (argc != 3)
	{
		std::cout << "./ircserv <port> <password>" << std::endl;
		return 1;
	}
	signal(SIGINT, sighandler);
    signal(SIGPIPE, sighandler); // attention pour l'instant je ne sais pas du tout ce qu'il faut faire en cas de SIGPIPE

    server.init()
    sockfd = get_listener_socket(argv[1]); //initialisation du socket d'ecoute sur le port passe en parametre
    if (sockfd < 0)
        ft_error("error getting a listening socket");
    add_fd(sockfd, &fds);
    
    //boucle server    
    while (stop == 0)
    {
        server.run();
           
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
                    std::cout << "New client wish to connect" << std::endl;
                    size = sizeof(distaddr);
                    newfd = accept(sockfd, &distaddr, &size);
                    if (newfd == -1)
                        ft_error("accept");
                    else
                    {
                        nbytes = recv(newfd, buff, sizeof(buff), 0);
                        ft_split(&test, buff);
                        std::cout << "taille du vector de string " << test.size() << std::endl;
                        for (unsigned long k = 0; k < test.size(); k++)
                        {
                            std::cout << "test[" << k << "] " << test[k] << std::endl; 
                            if (test[k] == "NICK")
                                send(newfd, RPL_WELCOME(test[k+1]).c_str(), RPL_WELCOME(test[k+1]).size(), 0);
                        }
                        if (nbytes < 0)
                            perror("recv");
                        else 
                        {
                            
                            if (send(newfd, buff, nbytes, 0) == -1) 
                                    perror("send");
                            add_fd(newfd, &fds);
                        }
                        
                    }
                }
                else // sinon c'est que c'est un client deja connecte qui nous dit qu'on a des trucs a lire
                {
                    std::string your_host = RPL_YOURHOST((std::string)SERVER_NAME, VERSION);
                    std::string my_info = RPL_MYINFO((std::string)SERVER_NAME, VERSION, USER_MODE, CHAN_MODE);
                    std::string creation = RPL_CREATED((std::string)"15/06/2022");
                    std::cout << "Client is sending data" << std::endl;
                    nbytes = recv(fds[i].fd, buff, sizeof(buff), 0);
                    ft_split(&test, buff);
                    std::cout << "taille du vector de string " << test.size() << std::endl;
                    for (unsigned long k = 0; k < test.size(); k++)
                    {
                        std::cout << "test[" << k << "] " << test[k] << std::endl; 
                        if (test[k] == "NICK")
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
                    send(fds[i].fd, buff, nbytes, 0);
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