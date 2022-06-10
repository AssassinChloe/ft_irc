/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassassi <cassassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 16:39:38 by cassassi          #+#    #+#             */
/*   Updated: 2022/06/10 16:40:14 by cassassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

void ft_error(std::string str)
{
    std::cout << str << std::endl;
    exit(1);
}

/*
    waaaah le beau serveeeeeur oulalala!
*/
int main()
{
    int sockfd, newfd, read_ret;
    int opt = 1;
    struct sockaddr_in  localaddr;
    socklen_t size = sizeof(localaddr);
    char buff[BUFFER_SIZE] = {0};
    
    struct sockaddr_in  distaddr;
    
    //l'instanciation de la struc sockaddr_in pour se connecter au port 8080 (MYPORT define dans le fichier .hpp)
    localaddr.sin_family = AF_INET;
    localaddr.sin_port = htons(MYPORT);
    localaddr.sin_addr.s_addr = INADDR_ANY;
    bzero(localaddr.sin_zero, 8);

    //on ouvre la chaussette    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if  (sockfd < 0)
        ft_error("error socket()");

    //on y mets des option auxquelles je comprends rien
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
        ft_error("error setsockopt()");
    
    //on bind la socket avec la struct localaddr pour que lefd sache avec quel port il bosse
    if (bind(sockfd, (struct sockaddr *)&localaddr, size) < 0)
        ft_error("error bind()");

    //on ecoute avec une file d'attente == BACKLOG
    if (listen(sockfd, BACKLOG) < 0)
        ft_error("error listen()");

    //boucle server    
    while (1)
    {
        //on accept la connexion entrante et on la stocke dans distaddr
        if ((newfd = accept(sockfd, (struct sockaddr *)&distaddr, &size)) < 0)
                ft_error("error accept()");
        
        //on lit et on envoie une confirmation de reception
        read_ret = read( newfd, buff, BUFFER_SIZE);
        std::cout << buff << std::endl;
        send(newfd, "message received\n", strlen("message received\n"), MSG_NOSIGNAL);
    }
    close(sockfd);
    std::cout << "end" << std::endl;
}