/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassassi <cassassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 16:39:38 by cassassi          #+#    #+#             */
/*   Updated: 2022/06/13 17:35:19 by cassassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

int stop = 0;

void ft_error(std::string str)
{
    std::cout << str << std::endl;
    exit(1);
}

void sighandler(int sig) { 
    if (sig == SIGINT)
        stop = 1; 
    if (sig == SIGPIPE)
        stop = 2;
    }

/*
    waaaah le beau serveeeeeur oulalala! C'EST CLAIR !!!!
*/
void *get_in_addr(struct sockaddr *sa)
{

    return &(((struct sockaddr_in*)sa)->sin_addr);
}

void add_to_pfds(struct pollfd **pfds, int newfd, int *fd_count, int *fd_size)
{
    // If we don't have room, add more space in the pfds array
    if (*fd_count == *fd_size) {
        *fd_size *= 2; // Double it

        *pfds = realloc(*pfds, sizeof(**pfds) * (*fd_size));
    }

    (*pfds)[*fd_count].fd = newfd;
    (*pfds)[*fd_count].events = POLLIN; // Check ready-to-read

    (*fd_count)++;
}

void del_from_pfds(struct pollfd *pfds, int i, int *fd_count)
{
    // Copy the one from the end over this one
    pfds[i] = pfds[*fd_count-1];

    (*fd_count)--;
}

int get_listener_socket()
{
    int listener;     // Listening socket descriptor
    int yes=1;        // For setsockopt() SO_REUSEADDR, below
    
    //var pour getaddrinfo
    struct addrinfo hints;
    struct addrinfo *res;
    struct addrinfo *tmp;
    char host[256] = {0};
    struct in_addr translate;
    
    //Initialisation de addrinfo
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_CANONNAME|AI_PASSIVE;
          
    if (getaddrinfo(NULL, PORT, &hints, &res) != 0)
        ft_error("error getaddrinfo");
    for (tmp = res; tmp != NULL; tmp = tmp->ai_next) 
    {
        listener = socket(AF_INET, SOCK_STREAM, tmp->ai_pr)
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
    freeaddrinfo(res); // All done with this

    // If we got here, it means we didn't get bound
    if (tmp == NULL)
        return -1;

    // Listen
    if (listen(listener, BACKLOG) == -1)
        return -1;
    return listener;
    
}

int main(/*int argc, char **argv*/)
{
    int i = 0;
    int sockfd, newfd, read_ret;
    int opt = 1;
    struct sockaddr_in  localaddr;
    struct sockaddr distaddr;
    socklen_t size = sizeof(localaddr);
    char buff[BUFFER_SIZE] = {0};
    char remoteIP[INET_ADDRSTRLEN];
    
    
    // var pour poll()
    struct pollfd *fds;
    int fd_size = 5;
    nfds_t fd_count = 0;
    int delai = 200;
    int nb_event;
    
    // pour lancement du programme suivant le sujet, a voir plus tard
    // if (argc != 3)
	// {
	// 	std::cout << "./ircserv <port> <password>" << std::endl;
	// 	return 1;
	// }
    // attention pour l'instant je ne sais pas du tout ce qu'il faut faire en cas de SIGPIPE
	signal(SIGINT, sighandler);
    signal(SIGPIPE, sighandler);
    
    fds = malloc(sizeof(*fds) * size_fds);
    if (!fds)
        ft_error("erreur malloc");
    listener = get_listener_socket();
    if (listener < 0)
        ft_error("error getting a listening socket");
    
    //initialisation des variable de poll()
    fds[0]->fd = listener;
    fds[0]->events = POLLIN;
    fd_counts++;
    
     
    // //l'instanciation de la struc sockaddr_in pour se connecter au port 8080 (MYPORT define dans le fichier .hpp)
    // localaddr.sin_family = AF_INET;
    // localaddr.sin_port = htons(MYPORT);
    // localaddr.sin_addr.s_addr = INADDR_ANY;
    // bzero(localaddr.sin_zero, 8);

    // //on ouvre la chaussette    
    // sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // if  (sockfd < 0)
    //     ft_error("error socket()");

    // //pour rendre accept() non bloquant (whatever it means)
    // fcntl(sockfd, F_SETFL, O_NONBLOCK);
    
    // //on y mets des option auxquelles je comprends rien
    // if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    //     ft_error("error setsockopt()");
    
    // //on bind la socket avec la struct localaddr pour que lefd sache avec quel port il bosse
    // if (bind(sockfd, (struct sockaddr *)&localaddr, size) < 0)
    //     ft_error("error bind()");

    // //on ecoute avec une file d'attente == BACKLOG
    // if (listen(sockfd, BACKLOG) < 0)
    //     ft_error("error listen()");
    
    //on accept la connexion entrante et on la stocke dans distaddr
    // if ((newfd = accept(sockfd, (struct sockaddr *)&distaddr, &size)) < 0)
    //             ft_error("error accept()");
    
   
    //boucle server    
    while (stop == 0)
    {   
            nb_event = poll(fds, nfds, -1)
            if (nb_event < 0)
            {
                perror("poll");
                ft_error("");
            }
        //tentative mais paul et moi on s'entend pas trop  
        for(int i = 0; i < fd_count; i++) 
        {
            if (fds[i].fd == listener) 
            {
                // If listener is ready to read, handle new connection
                size = sizeof(distaddr);
                newfd = accept(listener, &distaddr, &size);
                if (newfd == -1)
                    ft_error("accept");
                else
                    add_to_pfds(&fds, newfd, &fd_count, &fd_size);
            }
            else
            {
                int nbytes = recv(fds[i].fd, buff, sizeof(buff), 0);
                int sender_fd = fds[i].fd;
                if (nbytes <= 0) 
                {
                        // Got error or connection closed by client
                        if (nbytes == 0) {
                            // Connection closed
                            printf("pollserver: socket %d hung up\n", sender_fd);
                        } else {
                            perror("recv");
                        }

                        close(pfds[i].fd); // Bye!

                        del_from_pfds(pfds, i, &fd_count);

                } 
                else 
                {
                    // We got some good data from a client
                    for(int j = 0; j < fd_count; j++)
                    {
                        // Send to everyone!
                        int dest_fd = pfds[j].fd;
                        // Except the listener and ourselves
                        if (dest_fd != listener && dest_fd != sender_fd)
                        {
                            if (send(dest_fd, buf, nbytes, 0) == -1) 
                            {
                                perror("send");
                            }
                        }
                    }
                }
            }
        }
    }
  
        // on lit et on envoie une confirmation de reception
        read_ret = recv(newfd, buff, BUFFER_SIZE, 0);
        if (read_ret < 0)
            break;
        send(newfd, "test\n", strlen("test\n"), MSG_NOSIGNAL);
        std::cout << buff << std::endl;
        if (i++ == 4)
            break;
    close(sockfd);
    std::cout << "end" << std::endl;
}