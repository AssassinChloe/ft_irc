/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cassassi <cassassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 12:16:06 by cassassi          #+#    #+#             */
/*   Updated: 2022/06/14 11:35:53 by cassassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

int main(int ac, char **av)
{
    int loop = 1;
    if (ac != 2)
    {
        std::cout << "Usage : ./client \"your message\"" << std::endl;
        return (1);
    }
    
    int sock = 0;
    
    struct sockaddr_in  localaddr;
    char buffer[BUFFER_SIZE] = {0};
    std::string cin_buff;
    
    //On ouvre la socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << std::endl << " Socket creation error " << std::endl;
        return -1;
    }

    //l'instanciation de la struc sockaddr_in pour se connecter au port 8080 ecoute par le server
    localaddr.sin_family = AF_INET;
    localaddr.sin_port = htons(8080);
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
    recv(sock, buffer, BUFFER_SIZE, 0);
    std::cout << buffer << std::endl;
    while (loop == 1)
    {
        std::cin >> buffer;
        if (strcmp("exit", buffer) == 0)
            loop = 0;
        send(sock , buffer , strlen(buffer) , 0);
    }
    close(sock);
    return 0;
}