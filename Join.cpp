#include "Command.hpp"

// non gere :
// - rejoidreplusieurs channels dans une seule commande
// - channel avec key
// - nb max de channel qu'un client peut rejoindre
// - nb max de clients sur un channel

// a faire :
// - RPL -> faire liste des users
// - si parameters[0] == 0 (part all)
// - broadcast de l'info aux autres du channel


bool channelExist(Server *server, std::string chanName)
{
    int nb = server->getChannelNb();
    std::cout << "chan name a rechercher : " << chanName << ", taille _channels: " << nb << std::endl; // juste pour pas unused value

    for (int i = 0; i < nb; i++)
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

    if (this->getParameters().size() == 0)
    {
        std::string message = "JOIN :Not enough parameters\r\n"; // ERR_ #461 ERR_NEEDMOREPARAMS
        send_message(*this->client, message);
        return;
    }
		
	if (this->getParameters()[0] == "0")
    {
        std::string chan;
        std::map<std::string, std::string> tmp = this->client->getChanList();
        for (std::map<std::string, std::string>::iterator it = tmp.begin(); it != tmp.end(); it++)
        {
            chan = "PART " + (*it).first;
            Command command_line(*this->client, this->server, chan);
            command_line.execCommand();
        }
        return;
    }
		
    if (!channelExist(this->server, parameters[0]))
    {
        // std::cout << "channel a creer" << std::endl;
        server->addChannel(parameters[0]); // penser a ajouter le client qui cree dans la liste des operateurs du channel

    }
    if (channelExist(this->server, parameters[0]))
    { 
        int index = server->getChannelIndex(parameters[0]);
        server->getChannel(index).addClient(this->getClient()); // voir le ;ode par defaut

        // message JOIN
        std::string message = this->client->getPrefixe() + "JOIN :" + parameters[0] + "\r\n" ;
        send_message(*this->client, message);
        
        // message topic
        message = parameters[0] + " :" + server->getChannel(index).getTopic() + "\r\n";
        send_message(*this->client, message);

        // message users connectes
        message = this->client->getPrefixe() + "353 " + this->client->getUsername() + " = " + parameters[0] + " :@";
        std::map<int, Client*>  client_list = server->getChannel(index).getClientMap();
        for (std::map<int, Client*>::iterator it = client_list.begin(); it != client_list.end(); it++)
            message = message + (*it).second->getNickname() + " ";
        message = message + "\r\n";
        send_message(*this->client, message);

        // message end of list user
        message = this->client->getPrefixe() + "366 " + this->client->getUsername() + " " + parameters[0] + " :End of NAMES list\r\n"; // #366 RPL_ENDOFNAMES
        send_message(*this->client, message);

        message = this->client->getPrefixe() + "JOIN :" + parameters[0] + "\r\n" ;
        for (std::map<int, Client*>::iterator it = client_list.begin(); it != client_list.end(); it++)
        {
            if (this->client != (*it).second)
                {
                    int id = (*it).second->getFd();
                    send(id, message.c_str(), message.size(), MSG_NOSIGNAL);
                }
        }
        if (this->server->getChannel(index).getClients().size() == 1)
            this->client->addChannel(parameters[0], "O");
        else
            this->client->addChannel(parameters[0], "");

    }
    else // correspond a erreur car impossibe de creer le channel
    {
        std::cout << "channel non cree, client non ajoute" << std::endl; // reply 476 ERR_BADCHANMASK
        std::string message = parameters[0] + " :Bad Channel Mask\r\n";
        send_message(*this->client, message);
        // std::string ERR_BADCHANMASK(std::string channel) { return channel + " :Bad Channel Mask"; }
    }
    
}


// Note that this command also accepts the special argument of ("0", 0x30) instead of any of the usual parameters, 
// which requests that the sending client leave all channels they are currently connected to. 
// The server will process this command as though the client had sent a PART command for each channel they are a member of.

