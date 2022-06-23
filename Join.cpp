#include "Command.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "ft_irc.hpp"
#include <ctime>

bool channelExist(Server *server, std::string chanName)
{
    int nb = server->getChannelSize();
    std::cout << "chan name a rechercher : " << chanName << ", taille _channels: " << nb << std::endl; // juste pour pas unused value

    for (int i = 0; i<nb; i++)
    {
        std::cout << "i=" << i << "chaname = " << server->getChannel(i).getName() << std::endl;
        std::cout << "blob" << server->getChannel(0).getName()<< std::endl;
        if (server->getChannel(i).getName() == chanName)
            return (true);
    }
    //     std::cout << server->
    return (false);
}

void Command::Join()
{
    // check si le nom du serveur existe deja
     // cree le server si non existant
    if (!channelExist(this->server, parameters[0]))
    {
        std::cout << "channel a creer" << std::endl;
        server->addChannel(parameters[0]);

    }
    if (channelExist(this->server, parameters[0]))
    {
        // server->getChannel(parameters[0]).addClient(this->client);
        std::cout << "client a ajouter dans channel" << std::endl;
        std::cout << "broadcast d'arrivee du client aux autres connectes du channel" << std::endl;
    }
    else{
        std::cout << "channel non cree, client non ajoute" << std::endl;
    }
    // voir le retour si channel impossible a creer (genre bad name)
   
    // maintenant si channel existe enregistre le client dans le channel
    // si d'autres clients sur ce channel : diffuse info
}