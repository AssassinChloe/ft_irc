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

// ERR_INVITEONLYCHAN (473)
//   "<client> <channel> :Cannot join channel (+i)"
// Returned to indicate that a JOIN command failed because the channel is set to [invite-only] mode 
// and the client has not been invited to the channel or had an invite exception set for them. The text 
// used in the last param of this message may vary.

bool channelExist(Server *server, std::string chanName)
{
    int nb = server->getChannelNb();

    for (int i = 0; i < nb; i++)
    {
        if (server->getChannelName(i) == chanName)
            return (true);
    }
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
    std::string message;
    if (checkRegistration() != 0)
    {
        message = ERR_NOTREGISTERED(this->client->getPrefixe(), check_params(this->client->getNickname()));
        send_message(*this->client, message);
        return;
    }
    if (this->getParameters().size() == 0)
    {
        message = this->client->getPrefixe() + " 461 " + this->getClient().getNickname() + " " + "JOIN :Not enough parameters\r\n"; // ERR_ #461 ERR_NEEDMOREPARAMS
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

    std::vector<std::string> joinChan = ftsplit(parameters[0], ",");
    int nbChan = joinChan.size();
    for (int i = 0; i < nbChan; i++)
    {
        std::string LowChan = lowercase(joinChan[i]); // gestion case-sensitivity
        if (!channelExist(this->server, LowChan))
        {
            server->addChannel(LowChan); // penser a ajouter le client qui cree dans la liste des operateurs du channel

        }
        if (channelExist(this->server, LowChan))
        { 
            int index = server->getChannelIndex(LowChan);

            std::string modeChan = server->getChannel(index).getMode();
            // SI channel en mode i : verification de presence sur liste d'invites
            int isInvited = 1;
            if (searchIfMode('i', modeChan) == 1)
            {
                std::string modeClientSurServer = client->getStatus();
                if (searchIfMode('o', modeClientSurServer) == 1)
                    isInvited = 1;
                else
                    isInvited = 0;

                int nb = server->getChannel(index).getInvitedNb();
                for (int i=0; i<nb; i++)
                {
                    if (this->client->getNickname() == server->getChannel(index).getInvited(i) )
                        isInvited = 1;
                }
                if (isInvited == 0 )
                {
                    message =  LowChan + " :Cannot join channel (+i)\r\n";
                    send_message(*this->client, message); // ERR_INVITEONLYCHAN (473)
                }
            }
            if (isInvited == 1)
            {
                server->getChannel(index).addClient(this->getClient());
                std::map<int, Client*>  client_list = server->getChannel(index).getClientMap();
                message = this->client->getPrefixe() + "JOIN :" + LowChan + "\r\n" ;
                send_message(*this->client, message);

                if (this->server->getChannel(index).getClients().size() == 1)
                    this->client->addChannel(LowChan, "O");
                else
                    this->client->addChannel(LowChan, "+");

                message = LowChan + " :" + server->getChannel(index).getTopic() + "\r\n";
                send_message(*this->client, message);
                
                std::string list = "";

                for (std::map<int, Client*>::iterator it = client_list.begin(); it != client_list.end(); it++)
                {
                    if (searchIfMode(CHAN_USER_MODE, (*it).second->getChanMode(LowChan)) == 1
                    || searchIfMode(USER_MODE, (*it).second->getStatus()) == 1)
                        list = list + "@" + (*it).second->getNickname() + " ";
                    else
                        list = list + (*it).second->getNickname() + " ";
                }
                for (int j = 0; j < this->server->getChannel(index).getNbClients(); j++)
                {
                    Client tmp = *(this->server->getChannel(index).getClients()[j]);
                    std::string message = RPL_NAMREPLY(tmp.getPrefixe(), tmp.getNickname(), LowChan, list);
                    send_message(tmp, message);
                    message = RPL_ENDOFNAMES(tmp.getPrefixe(), tmp.getNickname(), LowChan);
                    send_message(tmp, message);
                }
                message = this->client->getPrefixe() + "JOIN :" + LowChan + "\r\n" ;
                for (std::map<int, Client*>::iterator it = client_list.begin(); it != client_list.end(); it++)
                {

                    if (this->client != (*it).second)
                        {
                            int id = (*it).second->getFd();
                            send(id, message.c_str(), message.size(), MSG_NOSIGNAL);
                        }
                }
                if (this->server->getChannel(index).getClients().size() == 1)
                    this->client->addChannel(LowChan, "O");
                else if (searchIfMode(USER_MODE, this->client->getStatus()) == 1)
                    this->client->addChannel(LowChan, "o");
                else
                    this->client->addChannel(LowChan, "");
            }

        }
        else // correspond a erreur car impossibe de creer le channel
        {
            std::string message = LowChan + " :Bad Channel Mask\r\n";  // reply 476 ERR_BADCHANMASK
            send_message(*this->client, message);
        }
    }
}


// Note that this command also accepts the special argument of ("0", 0x30) instead of any of the usual parameters, 
// which requests that the sending client leave all channels they are currently connected to. 
// The server will process this command as though the client had sent a PART command for each channel they are a member of.

// /join #test1,#test2,#test3,test,#test4
