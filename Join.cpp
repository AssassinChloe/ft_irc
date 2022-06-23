#include "Command.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "ft_irc.hpp"
#include <ctime>

// non gere :
// - rejoidreplusieurs channels dans une seule commande
// - channel avec key
// - nb max de channel qu'un client peut rejoindre
// - nb max de clients sur un channel


bool channelExist(Server *server, std::string chanName)
{
    int nb = server->getChannelSize();
    std::cout << "chan name a rechercher : " << chanName << ", taille _channels: " << nb << std::endl; // juste pour pas unused value

    for (int i = 0; i<nb; i++)
    {
        std::cout << "i=" << i << ", chaname = " << server->getChannelName(i) << std::endl;
        std::cout << "blob" << server->getChannelName(0)<< std::endl;
        if (server->getChannelName(i) == chanName)
            return (true);
    }
    //     std::cout << server->
    return (false);
}


// If a client’s JOIN command to the server is successful, the server MUST send, in this order:
// - A JOIN message with the client as the message <source> and the channel they have joined as the first 
//    parameter of the message.
// - The channel’s topic (with RPL_TOPIC (332) and optionally RPL_TOPICWHOTIME (333)), 
//    and no message if the channel does not have a topic.
// - A list of users currently joined to the channel (with one or more RPL_NAMREPLY (353) numerics 
//   followed by a single RPL_ENDOFNAMES (366) numeric). These RPL_NAMREPLY messages sent by the server MUST 
//   include the requesting client that has just joined the channel.



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
        // server->getChannel(parameters[0]).addClient(this->getClient()); // pbl dans le get channel avec string
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


// Note that this command also accepts the special argument of ("0", 0x30) instead of any of the usual parameters, 
// which requests that the sending client leave all channels they are currently connected to. 
// The server will process this command as though the client had sent a PART command for each channel they are a member of.