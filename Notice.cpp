// NOTICE message

// Command: NOTICE
//   Parameters: <target>{,<target>} <text to be sent>

// The NOTICE command is used to send notices between users, as well as to send notices to channels. 
// <target> is interpreted the same way as it is for the PRIVMSG command.

// The NOTICE message is used similarly to PRIVMSG. The difference between NOTICE and PRIVMSG is that 
// automatic replies must never be sent in response to a NOTICE message. This rule also applies to servers – 
// they must not send any error back to the client on receipt of a NOTICE command. The intention of this is 
// to avoid loops between a client automatically sending something in response to something it received. 
// This is typically used by ‘bots’ (a client with a program, and not a user, controlling their actions) 
// and also for server messages to clients.

// One thing for bot authors to note is that the NOTICE message may be interpreted differently by 
// various clients. Some clients highlight or interpret any NOTICE sent to a channel in the same way that a 
// PRIVMSG with their nickname gets interpreted. This means that users may be irritated by the use of 
// NOTICE messages rather than PRIVMSG messages by clients or bots, and they are not commonly used 
// by client bots for this reason.

#include "Command.hpp"

void    Command::Notice()
{
    
    int i = 0;
    int nb_param = parameters.size();

    if (nb_param == 0) 
            return;

    for (i=0; i<nb_param; i++)
    {

        if (parameters[i][0] == '#' || parameters[i][0] == '&' || parameters[i][0] == '+' || parameters[i][0] ==  '!')
        {


            std::string message = this->client->getPrefixe() + "PRIVMSG " +  parameters[i] + " :" + this->getArgLine() + " \r\n";
            std::cout << message << std::endl;

            int index = server->getChannelIndex(parameters[i]);

            if (index != -1) // = si le channel a ete trouve
            {

                // rajouter condition sur les droits

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
        }
        else
        {
            for (std::map<int, Client>::iterator it = this->server->getClientList().begin(); it!= this->server->getClientList().end(); it++)
            {
                if ((*it).second.getNickname() == this->parameters[i])
                {
                    int id = (it)->second.getFd();
                    std::string message = this->client->getPrefixe() + "PRIVMSG " + parameters[i] + " :" + this->getArgLine() + " \r\n";
                    send(id, message.c_str(), message.size(), 0);
                    std::cout <<"message individuel : " << message << std::endl;
                }
            }
        }     
    }
}