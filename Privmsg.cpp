#include "Command.hpp"

void    Command::Privmsg()
{
    
int i = 0;
int nb_param = parameters.size();

if (nb_param == 0) 
{
    // :lilin!liliu@localhost 411 lilin :No recipient given ()
    std::string message = this->client->getPrefixe() + " 411 " + this->client->getNickname() + parameters[i] + " :No recipient given ()\r\n";
    send_message(*this->client, message);
    return;
}

if (this->getArgLine().length() == 0)    
{
    // :lilin!liliu@localhost 412 lilin :No text to send
    std::string message = this->client->getPrefixe() + " 412 " + this->client->getNickname() + parameters[i] + " :No text to send\r\n";
    send_message(*this->client, message);
    return;
}   

for (i=0; i<nb_param; i++)
{

    if (parameters[i][0] == '#' || parameters[i][0] == '&' || parameters[i][0] == '+' || parameters[i][0] ==  '!')
    {

        std::string message = this->client->getPrefixe() + "PRIVMSG " +  parameters[i] + " :" + this->getArgLine() + " \r\n";
        std::cout << message << std::endl;

        int index = server->getChannelIndex(parameters[i]);

        if (index == -1) // = channel non trouve
        {
            std::string message = this->client->getPrefixe() + " 401 " + this->client->getNickname() + parameters[i] + " :No such nick/channel\r\n";
            send_message(*this->client, message); //ERR_NOSUCHNICK (401) // "<client> <nickname> :No such nick/channel"
        } 
        else
        {
            int condition = 1;
            // rajouter condition sur les droits ( a ajouter aussi a notice)
            if (condition)
            {
                std::map<int, Client*>  client_list = server->getChannel(index).getClientMap();
                for (std::map<int, Client*>::iterator it = client_list.begin(); it != client_list.end(); it++)
                {
                    if (this->client != (*it).second)
                        {
                            int id = (*it).second->getFd();
                            send(id, message.c_str(), message.size(), 0);
                        }
                }
            }
            

            else// else si pas de droits
            {
                std::string message = this->client->getPrefixe() + " " + parameters[i] + " :Cannot send to channel\r\n";
                send_message(*this->client, message);
                // ERR_CANNOTSENDTOCHAN (404) "<client> <channel> :Cannot send to channel"
            }
        }
    }
    else
    {
        int find = 0;
        for (std::map<int, Client>::iterator it = this->server->getClientList().begin(); it!= this->server->getClientList().end(); it++)
        {
            if ((*it).second.getNickname() == this->parameters[i])
            {
                int id = (it)->second.getFd();
                std::string message = this->client->getPrefixe() + "PRIVMSG " + parameters[i] + " :" + this->getArgLine() + " \r\n";
                send(id, message.c_str(), message.size(), 0);
                find = 1;
                std::cout <<"message individuel : " << message << std::endl;
            }
        }
        if (find == 0)
        {
            std::string message = this->client->getPrefixe() + " 401 " + this->client->getNickname() + parameters[i] + " :No such nick/channel\r\n";
            send_message(*this->client, message); //ERR_NOSUCHNICK (401) // "<client> <nickname> :No such nick/channel"
        }
    }     
}
}


// PRIVMSG message

//      Command: PRIVMSG
//   Parameters: <target>{,<target>} <text to be sent>

// The PRIVMSG command is used to send private messages between users, as well as to send messages to channels. 
// <target> is the nickname of a client or the name of a channel.

// If <target> is a channel name and the client is banned and not covered by a ban exception, 
// the message will not be delivered and the command will silently fail. Channels with the moderated mode active may 
// block messages from certain users. Other channel modes may affect the delivery of the message or cause the message 
// to be modified before delivery, and these modes are defined by the server software and configuration being used.

// If a message cannot be delivered to a channel, the server SHOULD respond with an ERR_CANNOTSENDTOCHAN (404) 
// numeric to let the user know that this message could not be delivered.

// If <target> is a channel name, it may be prefixed with one or more channel membership prefix character (@, +, etc) 
// and the message will be delivered only to the members of that channel with the given or higher status in the channel. 
// Servers that support this feature will list the prefixes which this is supported for in the STATUSMSG RPL_ISUPPORT 
// parameter, and this SHOULD NOT be attempted by clients unless the prefix has been advertised in this token.

// If <target> is a user and that user has been set as away, the server may reply with an RPL_AWAY (301) numeric 
// and the command will continue.

// The PRIVMSG message is sent from the server to client to deliver a message to that client. 
// The <source> of the message represents the user or server that sent the message, and the <target> represents 
// the target of that PRIVMSG (which may be the client, a channel, etc).

// When the PRIVMSG message is sent from a server to a client and <target> starts with a dollar character ('$', 0x24), 
// the message is a broadcast sent to all clients on one or multiple servers.

// Numeric Replies:

//     ERR_NOSUCHNICK (401)
//     ERR_NOSUCHSERVER (402)
//     ERR_CANNOTSENDTOCHAN (404)
//     ERR_TOOMANYTARGETS (407)
//     ERR_NORECIPIENT (411)
//     ERR_NOTEXTTOSEND (412)
//     ERR_NOTOPLEVEL (413)
//     ERR_WILDTOPLEVEL (414)
//     RPL_AWAY (301)


// ERR_CANNOTSENDTOCHAN (404)

//   "<client> <channel> :Cannot send to channel"

// Indicates that the PRIVMSG / NOTICE could not be delivered to <channel>. The text used in the last param of 
// this message may vary.

// This is generally sent in response to channel modes, such as a channel being moderated and the client not having 
// permission to speak on the channel, or not being joined to a channel with the no external messages mode set.