#include "Command.hpp"

void    Command::Invite()
{
    if (this->getParameters().size() == 0)
    {
        std::string message = "INVITE :Not enough parameters\r\n"; // ERR_ #461 ERR_NEEDMOREPARAMS
        send_message(*this->client, message);
        return;
    }

    int index = this->server->getChannelIndex(parameters[1]);
    if (index == -1 || server->getChannel(index).getNbClients() == 0)
    {
        std::string message = parameters[1] + " :No such channel\r\n"; // ERR_NOSUCHCHANNEL (403)
        send_message(*this->client, message);
        return;
    }

    if (server->getChannel(index).isOnChannel(this->getClient().getNickname())) // client sur le channel
    {
        // SI client deja sur le channel ERR_USERONCHANNEL
        if (server->getChannel(index).isOnChannel(parameters[0]))
        {
            std::string message = this->client->getPrefixe() + " 443 " + parameters[0] + " " + parameters[1] +  " :is already on channel\r\n"; //"<client> <nick> <channel> :is already on channel"
            send_message(*this->client, message); //ERR_USERONCHANNEL (443)
            return;
        }
        
        std::string modeChan = server->getChannel(index).getMode();
        std::cout << "mode Chan = " << modeChan << std::endl;
        // SI channel en mode i : verification de has power d'inviter = (client est operator channel)
        if (searchIfMode('i', modeChan) == 1)
        {
            // std::string modeClient = server->getChannel(index).getClientMode(&(this->client));
            std::string modeClient = server->getChannel(index).getClientMode(server->getClient(parameters[0]));
            // std::string Channel::getClientMode(Client &client) { return client_mode[client.getFd()]; }
            // Client &getClient(std::string name);
            std::cout << "mode Client = " << modeClient << std::endl;
            if (!(searchIfMode('o', modeClient) == 1 || searchIfMode('O', modeClient) == 1 ))
            {
                std::string message =  parameters[1] + " :You're not channel operator\r\n";
                send_message(*this->client, message);
                return;  // ERR_CHANOPRIVSNEEDED 482
            }
        }
        // verification que le nickname existe sur le server (pas demande mais sinon je ne vois pas comment faire)
        int NickOnServer = 0;
        for (std::map<int, Client>::iterator it = this->server->getClientList().begin(); it != this->server->getClientList().end(); it++)
        {
            if ((*it).second.getNickname() == this->parameters[0])
            {
               NickOnServer = 1;
            } 
        }
        if (NickOnServer == 0) // pas trouve de message approprie
        {
            std::cout << "NickName inconnu" << std::endl;
            return;
        }
        // ajout du nickname dans la liste des invite
        server->getChannel(index).addInvited(parameters[0]);
        // envoi au demandeur RPL_INVITING numeric
        std::string message =  parameters[0] + " " + parameters[1] + "\r\n"; 
        send_message(*this->client, message); //pas considere comme garbage mais selble bien simple
        // envoi de l'invitation a l'invite an INVITE message with the issuer as <source>, to the target user 
        class Client &invited_client = server->getClient(parameters[0]);
        std::string message2 = this->client->getPrefixe() + " INVITE " + parameters[0] + " " + parameters[1] + "\r\n";
        send_message(invited_client, message2);

    }
    else // client pas sur le channel
    {
        std::string message = this->client->getPrefixe() +" 442 "+ this->client->getNickname() + " " + parameters[1] + " :You're not on that channel\r\n";
        send_message(*this->client, message); // ERR_NOTONCHANNEL (442)
    }
}

// int searchIfMode(char tofind, std::string target);


// INVITE message
//      Command: INVITE
//   Parameters: <nickname> <channel>

// The INVITE command is used to invite a user to a channel. The parameter <nickname> is the nickname of the person 
// to be invited to the target channel <channel>.

// The target channel SHOULD exist (at least one user is on it). Otherwise, the server SHOULD reject the command with 
// the ERR_NOSUCHCHANNEL numeric.

// Only members of the channel are allowed to invite other users. Otherwise, the server MUST reject the command with 
// the ERR_NOTONCHANNEL numeric.

// Servers MAY reject the command with the ERR_CHANOPRIVSNEEDED numeric. In particular, they SHOULD reject it when the 
// channel has invite-only mode set, and the user is not a channel operator.

// If the user is already on the target channel, the server MUST reject the command with the ERR_USERONCHANNEL numeric.

// When the invite is successful, the server MUST send a RPL_INVITING numeric to the command issuer, and an INVITE message, 
// with the issuer as <source>, to the target user. Other channel members SHOULD NOT be notified.

// Numeric Replies:

//     RPL_INVITING (341)
//     ERR_NEEDMOREPARAMS (461)
//     ERR_NOSUCHCHANNEL (403)
//     ERR_NOTONCHANNEL (442)
//     ERR_CHANOPRIVSNEEDED (482)
//     ERR_USERONCHANNEL (443)

// Command Examples:
//   INVITE Wiz #foo_bar    ; Invite Wiz to #foo_bar

// Message Examples:
//   :dan-!d@localhost INVITE Wiz #test    ; dan- has invited Wiz
//                                         to the channel #test

// Invite list

// Servers MAY allow the INVITE with no parameter, and reply with a list of channels the sender is invited to as 
// RPL_INVITELIST (336) numerics, ending with a RPL_ENDOFINVITELIST (337) numeric.

// ERR_CHANOPRIVSNEEDED (482)
//   "<client> <channel> :You're not channel operator"
// Indicates that a command failed because the client does not have the appropriate channel privileges. 
// This numeric can apply for different prefixes such as halfop, operator, etc. The text used in the last param 
// of this message may vary.

// RPL_INVITING (341)
//   "<client> <nick> <channel>"
// Sent as a reply to the INVITE command to indicate that the attempt was successful and the client with the 
// nickname <nick> has been invited to <channel>.

// ERR_USERONCHANNEL (443)
//   "<client> <nick> <channel> :is already on channel"
// Returned when a client tries to invite <nick> to a channel they’re already joined to.