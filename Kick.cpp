

#include "Command.hpp"

// ligne d'argument : parameters[0] = server OU #... (a ne pas mettre par l'utilisateur, fait par client si hexchat)
//                    parameters[1] : user OU user1,user2 -> a split en cas de virgule voir ftsplit
//                    argLine = tous les mots qui apparaissent apres les users (le client met les :avant)

void    Command::Kick()
{
        
    int nb_param = parameters.size();
    std::string message;
	if (checkRegistration() != 0)
    {
        message = ERR_NOTREGISTERED(this->client->getPrefixe(), check_params(this->client->getNickname()));
        send_message(*this->client, message);
        return;
    }
    if (nb_param <= 1) 
    {
        message = this->client->getPrefixe() + " 461 " + this->getClient().getNickname() + " "  + "KICK :Not enough parameters\r\n"; // ERR_NEEDMOREPARAMS (461)
        send_message(*this->client, message);
        return;
    }
    
    else
    {
        parameters[0] = lowercase(parameters[0]); // gestion case-sensitivity
        int index = server->getChannelIndex(parameters[0]); // si channel n'existe pas = -1
        if (index == -1)
        {
            message = this->client->getPrefixe() + " 403 " + this->getClient().getNickname() + " " + parameters[0] + " :No such channel\r\n"; // ERR_NOSUCHCHANNEL (403)
            send_message(*this->client, message);
            return;
        }
        std::string modeClientSurChan = client->getChanMode(parameters[0]);
        std::string modeClientSurServer = client->getStatus();
        if (searchIfMode('o', modeClientSurServer) != 1  && (modeClientSurChan.size() == 0 || !(searchIfMode('o', modeClientSurChan) == 1 || searchIfMode('O', modeClientSurChan) == 1 ))) // client n'a pas les droits sur le channel
        {
            message = this->client->getPrefixe() + " 482 " + this->getClient().getNickname() + " " + parameters[0] + " :You're not channel operator\r\n";
            send_message(*this->client, message); //ERR_CHANOPRIVSNEEDED (482)
            return;

        }

        std::vector<std::string> kickedClients = ftsplit(parameters[1], ",");
        int nbKicked = kickedClients.size();
        for (int i = 0; i < nbKicked; i++)
        {
            int find = 0;

            std::map<int, Client*>  client_list = server->getChannel(index).getClientMap();
            for (std::map<int, Client*>::iterator it = client_list.begin(); it != client_list.end(); it++)
            {
                if (it->second->getNickname() == kickedClients[i])
                {
                    message = this->client->getPrefixe() + " KICK " + parameters[0] + " " + kickedClients[i] + " ";
                    if (argLine.length() != 0)
                        message = message + ":" + argLine;
                    message = message + "\r\n";
                    server->getChannel(index).broadcast(message);


                    std::string modeChan = server->getChannel(index).getMode();
                    if (searchIfMode('i', modeChan) == 1)
                    {
                        int nb = server->getChannel(index).getInvitedNb();
                        for (int j=0; j<nb; j++)
                        {
                            if (kickedClients[i] == server->getChannel(index).getInvited(j) )
                                server->getChannel(index).delInvited(kickedClients[i]);
                        }

                    }
                    server->getChannel(index).removeClient(kickedClients[i]);

                    // check de si le channel est vide et remove le channel du server
	                if(server->getChannel(index).getNbClients() == 0)
                        server->delChannel(parameters[0]);

                    find = 1;
                }
            }
            if (find == 0)
            {
                message = kickedClients[i] + " " + parameters[0] + " :They aren't on that channel\r\n";
                send_message(*this->client, message); // ERR_USERNOTINCHANNEL (441)
            }

        }

    }
}




// KICK message

//       Command: KICK
//    Parameters: <channel> <user> *( "," <user> ) [<comment>]

// The KICK command can be used to request the forced removal of a user from a channel. 
// It causes the <user> to be removed from the <channel> by force. If no comment is given, 
// the server SHOULD use a default message instead.

// The server MUST NOT send KICK messages with multiple users to clients. This is necessary 
// to maintain backward compatibility with existing client software.

// Servers MAY limit the number of target users per KICK command via the TARGMAX parameter 
// of RPL_ISUPPORT, and silently drop targets if the number of targets exceeds the limit.

// Numeric Replies:

//     ERR_NEEDMOREPARAMS (461)
//     ERR_NOSUCHCHANNEL (403)
//     ERR_CHANOPRIVSNEEDED (482)
//     ERR_USERNOTINCHANNEL (441)
//     ERR_NOTONCHANNEL (442)


// Examples:

//    KICK #Finnish Matthew           ; Command to kick Matthew from
//                                    #Finnish

//    KICK &Melbourne Matthew         ; Command to kick Matthew from
//                                    &Melbourne

//    KICK #Finnish John :Speaking English
//                                    ; Command to kick John from #Finnish
//                                    using "Speaking English" as the
//                                    reason (comment).

//    :WiZ!jto@tolsun.oulu.fi KICK #Finnish John
//                                    ; KICK message on channel #Finnish
//                                    from WiZ to remove John from channel