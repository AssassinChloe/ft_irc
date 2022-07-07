#include "Command.hpp"

void Command::part()
{
    if (parameters.size() ==0)
    {
        std::string message = "PART :Not enough parameters\r\n"; // ERR_ #461 ERR_NEEDMOREPARAMS
        send_message(*this->client, message);
        return;
    }

    std::vector<std::string> partChan = ftsplit(parameters[0], ",");
    int nbChan = partChan.size();
    for (int i = 0; i < nbChan; i++)
    {

        int index = server->getChannelIndex(partChan[i]);
        if (index == -1)
        {
            std::string message = partChan[i] + " :No such channel\r\n"; 
            send_message(*this->client, message); // ERR_NOSUCHCHANNEL (403)
        }
        else
        {
            std::map<int, Client*>  client_list = server->getChannel(index).getClientMap();
            if (server->getChannel(index).isOnChannel(this->getClient().getNickname())) // client sur le channel
            {
                std::string message = this->client->getPrefixe() + "PART :" + partChan[i] + " " + this->getArgLine() + "\r\n";
                for (std::map<int, Client*>::iterator it = client_list.begin(); it != client_list.end(); it++)
                {
                    send_message(*(*it).second, message);
                }
                this->server->getChannel(index).removeClient(*this->client);
                if (this->server->getChannel(index).getNbClients() == 0)
                    this->server->delChannel(partChan[i]);
                this->server->getChannel(index).delInvited(this->client->getNickname()); // suppression de la list des invites, a voir si on conserve
                this->client->delChannel(partChan[i]);
            }
            else
            {
                std::string message2 = this->client->getPrefixe() +" 442 "+ this->client->getNickname() + " " + partChan[i] + " :You're not on that channel\r\n"; // # define ERR_NOTONCHANNEL(channel) (442)
                send_message(*this->client, message2); // ERR_NOTONCHANNEL (442)
            }
        }
    }
}

// PART message

//      Command: PART
//   Parameters: <channel>{,<channel>} [<reason>]

// The PART command removes the client from the given channel(s). On sending a successful PART command, the user will receive a 
// PART message from the server for each channel they have been removed from. <reason> is the reason that the client has left 
// the channel(s).

// For each channel in the parameter of this command, if the channel exists and the client is not joined to it, they will 
// receive an ERR_NOTONCHANNEL (442) reply and that channel will be ignored. If the channel does not exist, the client will 
// receive an ERR_NOSUCHCHANNEL (403) reply and that channel will be ignored.

// This message may be sent from a server to a client to notify the client that someone has been removed from a channel. In 
// this case, the message <source> will be the client who is being removed, and <channel> will be the channel which that client 
// has been removed from. Servers SHOULD NOT send multiple channels in this message to clients, and SHOULD distribute these 
// multiple-channel PART messages as a series of messages with a single channel name on each. If a PART message is distributed 
// in this way, <reason> (if it exists) should be on each of these messages.

// Numeric Replies:

//     ERR_NEEDMOREPARAMS (461)
//     ERR_NOSUCHCHANNEL (403)
//     ERR_NOTONCHANNEL (442)
