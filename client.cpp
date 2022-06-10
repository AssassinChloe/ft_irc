/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassassi <cassassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 12:16:06 by cassassi          #+#    #+#             */
/*   Updated: 2022/06/10 16:33:39 by cassassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

int main(int ac, char **av)
{
    if (ac != 2)
    {
        std::cout << "Usage : ./client \"your message\"" << std::endl;
        return (1);
    }
    
    int sock = 0, valread;
    
    struct addrinfo hints;
    struct addrinfo *res;
    struct addrinfo *tmp;
    char host[256] = {0};
    
    struct sockaddr_in  localaddr;
    char buffer[BUFFER_SIZE] = {0};
    
    //On ouvre la socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << std::endl << " Socket creation error " << std::endl;
        return -1;
    }
   
    // Ici on arrive a convertir une adresse internet en ip mais je sais pas comment l'utiliser
    //Je sais que c'est dans le client mais elle est dans la liste des fonction a utiliser alors
    //je voulais voir si j'arrivais a en faire quelque chose
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_CANONNAME;
    if (getaddrinfo("localhost", NULL, &hints, &res) != 0)
        std::cout << std::endl << "error getaddrinfo" << std::endl;
    for (tmp = res; tmp != NULL; tmp = tmp->ai_next) 
    {
        getnameinfo(tmp->ai_addr, tmp->ai_addrlen, host, sizeof(host), NULL, 0, NI_NUMERICHOST);   
        std::cout << " host* " << host << std::endl;
        std::cout << " cname* " << tmp->ai_canonname << std::endl;
    }

    //l'instanciation de la struc sockaddr_in pour se connecter au port 8080 ecoute par le server
    localaddr.sin_family = AF_INET;
    localaddr.sin_port = htons(MYPORT);
    localaddr.sin_addr.s_addr = INADDR_ANY;
    bzero(localaddr.sin_zero, 8);
    
    //connexion au serveur
    if (connect(sock, (struct sockaddr *)&localaddr, sizeof(struct sockaddr)) < 0)
    {
        std::cout << std::endl << "connexion failed" << std::endl;
        return -1;
    }
    
    //envoi du message et accuse reception
    send(sock , av[1] , strlen(av[1]) , 0);
    valread = read( sock , buffer, 1024);
    if (valread < 0)
        std::cout << "error read" << std::endl;
    std::cout << buffer << std::endl;
    
    freeaddrinfo(res);
    close(sock);
    return 0;
}