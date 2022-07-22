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
    std::string message;
	if (checkRegistration() != 0)
    {
        message = ERR_NOTREGISTERED(this->client->getPrefixe(), check_params(this->client->getNickname()));
        send_message(*this->client, message);
        return;
    }
    if (nb_param == 0) 
            return;

    std::vector<std::string> target = ftsplit(parameters[0], ",");
    int nbtarget = target.size();
    for (i=0; i<nbtarget; i++)
    {

        if (target[i][0] == '#' || target[i][0] == '&' || target[i][0] == '+' || target[i][0] ==  '!')
        {

            target[i] = lowercase(target[i]); // gestion case-sensitivity
            message = this->client->getPrefixe() + "PRIVMSG " +  target[i] + " :" + this->getArgLine() + " \r\n";
            int index = server->getChannelIndex(target[i]);

            if (index != -1) // = si le channel a ete trouve
            {

                //condition sur les droits
                int condition = 1;
                int index = server->getChannelIndex(target[i]);
                std::string modeChan = server->getChannel(index).getMode();
                if (searchIfMode('n', modeChan) == 1 && (server->getChannel(index).isOnChannel(client->getNickname()) == 0))
                    condition = 0;

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
            }
        }
        else
        {
            for (std::map<int, Client>::iterator it = this->server->getClientList().begin(); it!= this->server->getClientList().end(); it++)
            {
                if ((*it).second.getNickname() == target[i])
                {
                    int id = (it)->second.getFd();
                    message = this->client->getPrefixe() + "PRIVMSG " + target[i] + " :" + this->getArgLine() + " \r\n";
                    send(id, message.c_str(), message.size(), 0);
                }
            }
        }     
    }
}