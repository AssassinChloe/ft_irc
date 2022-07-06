#include "Command.hpp"
#include <string>
#include <sstream>

void    Command::Topic()
{

    // int i = 0;
    int nb_param = parameters.size();

    if (nb_param == 0) 
    {
        std::string message = parameters[0] + " :Not enough parameters\r\n"; // ERR_NEEDMOREPARAMS (461)
        send_message(*this->client, message);
        return;
    }
    
    else
    {
        // trouver le channel
        int index = server->getChannelIndex(parameters[0]); // si channel n'existe pas = -1
        if (index == -1)
        {
            std::string message = parameters[0] + " :No such channel\r\n"; // ERR_NOSUCHCHANNEL (403)
            send_message(*this->client, message);
            return;
        }
        if (server->getChannel(index).isOnChannel(this->getClient().getNickname())) //si client sur le channel
        {
            // voir si n'a  pas les droits pour changer
            std::string modeChan = server->getChannel(index).getMode();
            std::cout << "mode Chan = " << modeChan << std::endl;
            // SI channel en mode t : verification de has power = (client est operator channel)
            if (searchIfMode('t', modeChan) == 1)
            {
                std::string modeClient = server->getChannel(index).getClientMode(this->getClient());
                std::cout << "mode Client = " << modeClient << std::endl;
                if (!(searchIfMode('o', modeClient) == 1 || searchIfMode('O', modeClient) == 1 ))
                {
                    std::string message =  parameters[0] + " :You're not channel operator\r\n";
                    send_message(*this->client, message);
                    return;  // ERR_CHANOPRIVSNEEDED 482
                }
            }

            // si a les droit
            if (nb_param == 1 && argLine == "") //normalement comportement diffreent si existe : ou pas... voir si a preciser
            {
                if (server->getChannel(index).getTopic().length() != 0) // topic existe
                {
                    std::string message = parameters[0] + " :" + server->getChannel(index).getTopic() + "\r\n";
                    // std::cout << "message TOPIC :" << message <<std::endl;
                    send_message(*this->client, message);
                }
                else 
                {
                    std::string message = parameters[0] + " :No topic is set\r\n";
                    // std::cout << "message NO_TOPIC :" << message <<std::endl;
                    send_message(*this->client, message);
                }
                // define ERR_NOTOPIC(channel) (channel + " :No topic is set\r\n")
            }
            if (nb_param == 1 && argLine.length() != 0)
            {
                server->getChannel(index).setTopic(argLine);
                server->getChannel(index).setTopicSetter(this->client);
                server->getChannel(index).setLastTopicSet();
                std::cout << "topic set on " << server->getChannel(index).getLastTopicSet() << " by " << server->getChannel(index).getTopicSetter()->getNickname() << std::endl;
                // voir si ajout de RPL_TOPICWHOTIME (333) --> garbage ???
                std::stringstream ss;
                ss << server->getChannel(index).getLastTopicSet();
                std::string timestring = ss.str();
                std::string message = this->client->getPrefixe() +" 333 "+ " server " + parameters[0] + " " ;
                message = message + server->getChannel(index).getTopicSetter()->getNickname() + " " ;
                message = message + timestring +  "\r\n";
                // message = message + server->getChannel(index).getLastTopicSet() +  "\r\n";
                // std::cout << "message TOPICWHOTIME :" << message <<std::endl;
                send_message(*this->client, message); // message 

                std::map<int, Client*>  client_list = server->getChannel(index).getClientMap();
                for (std::map<int, Client*>::iterator it = client_list.begin(); it != client_list.end(); it++)
                {
                    // if (this->client != (*it).second) // condition a mettre si on ne veut pas envoyer a celui qui a change le topic
                    // {
                        std::string message = this->client->getPrefixe() +" TOPIC "+parameters[0] + " :" + server->getChannel(index).getTopic() + "\r\n";
                        // :lilin!liliu@localhost TOPIC #test :i love rockn roll
                        int id = (*it).second->getFd();
                        send(id, message.c_str(), message.size(), 0);
                    // }
                }
            }
        }

        else // client pas sur le channel
        {
            std::string message = this->client->getPrefixe() +" 442 "+ this->client->getNickname() + " " + parameters[0] + " :You're not on that channel\r\n"; // # define ERR_NOTONCHANNEL(channel) (442)
            // :lilin!liliu@localhost 442 lilin #test :You're not on that channel
            send_message(*this->client, message);
        }
        // verifier que le client est dessus
        // 
        // si oui, 
        //      si nb_param = 1 : envoyer (RPL Topic + RPL_TOPICWHOTIME) ou rpl notopic
        //      si nb_param = 2 : 
        //          si client n'a pa les droits pour changer ERR_CHANOPRIVSNEEDED (482)
        //          si pas de soucis, changement du topic + envoi commande TOPIC a tout le channel

        // si non ERR_NOTONCHANNEL (442)
    }
    
}


// Command: TOPIC
//   Parameters: <channel> [<topic>]

// The TOPIC command is used to change or view the topic of the given channel. 
// If <topic> is not given, either RPL_TOPIC or RPL_NOTOPIC is returned specifying the current channel topic or lack of one. 
// If <topic> is an empty string, the topic for the channel will be cleared.

// If the client sending this command is not joined to the given channel, and tries to view its’ topic, 
// the server MAY return the ERR_NOTONCHANNEL (442) numeric and have the command fail.

// If RPL_TOPIC is returned to the client sending this command, RPL_TOPICWHOTIME SHOULD also be sent to that client.

// If the protected topic mode is set on a channel, then clients MUST have appropriate channel permissions 
// to modify the topic of that channel. If a client does not have appropriate channel permissions and tries to change the topic, 
// the ERR_CHANOPRIVSNEEDED (482) numeric is returned and the command will fail.

// If the topic of a channel is changed or cleared, every client in that channel 
// (including the author of the topic change) will receive a TOPIC command with the new topic as argument 
// (or an empty argument if the topic was cleared) alerting them to how the topic has changed.

// Clients joining the channel in the future will receive a RPL_TOPIC numeric (or lack thereof) accordingly.

// Numeric Replies:

//     ERR_NEEDMOREPARAMS (461)
//     ERR_NOSUCHCHANNEL (403)
//     ERR_NOTONCHANNEL (442)
//     ERR_CHANOPRIVSNEEDED (482)
//     RPL_NOTOPIC (331)
//     RPL_TOPIC (332)
//     RPL_TOPICWHOTIME (333)

// Command Examples:

//   TOPIC #test :New topic          ; Setting the topic on "#test" to
//                                   "New topic".

//   TOPIC #test :                   ; Clearing the topic on "#test"

//   TOPIC #test                     ; Checking the topic for "#test"

// # define RPL_NOTOPIC(channel) (channel + " :No topic is set\r\n") // 331 
// # define RPL_TOPIC(channel, subject) (channel + " :" + subject + "\r\n") // 332 

// RPL_TOPICWHOTIME (333)
//   "<client> <channel> <nick> <setat>"
// Sent to a client to let them know who set the topic (<nick>) and when they set it (<setat> is a unix timestamp). Sent after RPL_TOPIC (332).