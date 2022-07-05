// a renvoyer : :vmercier!vmercier@localhost 324 vmercier #test +n 

// Command: MODE
//   Parameters: <target> [<modestring> [<mode arguments>...]]

// The MODE command is used to set or remove options (or modes) from a given target.

// User mode

// If <target> is a nickname that does not exist on the network, the ERR_NOSUCHNICK (401) numeric is returned. 
// If <target> is a different nick than the user who sent the command, the ERR_USERSDONTMATCH (502) numeric is returned.

// If <modestring> is not given, the RPL_UMODEIS (221) numeric is sent back containing the current modes of 
// the target user.

// If <modestring> is given, the supplied modes will be applied, and a MODE message will be sent to the user 
// containing the changed modes. If one or more modes sent are not implemented on the server, the server MUST apply 
// the modes that are implemented, and then send the ERR_UMODEUNKNOWNFLAG (501) in reply along with the MODE message.

// Channel mode

// If <target> is a channel that does not exist on the network, the ERR_NOSUCHCHANNEL (403) numeric is returned.

// If <modestring> is not given, the RPL_CHANNELMODEIS (324) numeric is returned. Servers MAY choose to hide 
// sensitive information such as channel keys when sending the current modes. Servers MAY also return the 
// RPL_CREATIONTIME (329) numeric following RPL_CHANNELMODEIS.

// If <modestring> is given, the user sending the command MUST have appropriate channel privileges on the target 
// channel to change the modes given. If a user does not have appropriate privileges to change modes on 
// the target channel, the server MUST not process the message, and ERR_CHANOPRIVSNEEDED (482) numeric is returned. 
// If the user has permission to change modes on the target, the supplied modes will be applied based on the type 
// of the mode (see below). For type A, B, and C modes, arguments will be sequentially obtained from <mode arguments>. 
// If a type B or C mode does not have a parameter when being set, the server MUST ignore that mode. If a type A mode 
// has been sent without an argument, the contents of the list MUST be sent to the user, unless it contains 
// sensitive information the user is not allowed to access. When the server is done processing the modes, a MODE command 
// is sent to all members of the channel containing the mode changes. Servers MAY choose to hide sensitive information 
// when sending the mode changes.

#include "Command.hpp"

void    Command::Mode()
{
    if (this->parameters.size() == 1) // a modifier pour user mode ou channel mode
    {
        int index = this->server->getChannelIndex(this->parameters[0]);
        if ( index >= 0)
        {
            std::string message = this->client->getPrefixe() + "324 " +  this->client->getNickname() + " " + this->parameters[0] + " " + this->server->getChannel(index).getMode() + " \r\n" ;
            // std::cout << "Message " << message << std::endl;
            send_message(*this->client, message);
            return;
        }
        else // attention a modifier (cas prevu avec juste un argument pour utilisateur, mais peut avoir plusieurs ?)
        {
            int find = 0;
            for (std::map<int, Client>::iterator it = this->server->getClientList().begin(); it!= this->server->getClientList().end(); it++)
            {
                if ((*it).second.getNickname() == this->parameters[0])
                {
                    // chaner le mode du user

                    find = 1;
                }
            }
            if (find == 0)
            {
                std::string message = this->client->getPrefixe() + " 401 " + this->client->getNickname() + parameters[0] + " :No such nick/channel\r\n";
                send_message(*this->client, message);
                //ERR_NOSUCHNICK (401) // "<client> <nickname> :No such nick/channel"
            }
        }
    }
}

//324 std::string RPL_CHANNELMODEIS(std::string channel, std::string mode, std::string params) { return channel + " " + mode + " " + params; }
//403 std::string ERR_NOSUCHCHANNEL(std::string channel) { return channel + " :No such channel"; }
//401 std::string ERR_NOSUCHNICK(std::string nickname) { return nickname + " :No such nick/channel"; }
//501 std::string ERR_UMODEUNKNOWNFLAG() { return ":Unknown MODE flag"; }

// Command Examples:

//   MODE dan +i                     ; Setting the "invisible" user mode on dan.

//   MODE #foobar +mb *@127.0.0.1    ; Setting the "moderated" channel mode and
//                                   adding the "*@127.0.0.1" mask to the ban
//                                   list of the #foobar channel.

// Message Examples:

//   :dan!~h@localhost MODE #foobar -bl+i *@192.168.0.1
//                                   ; dan unbanned the "*@192.168.0.1" mask,
//                                   removed the client limit from, and set the
//                                   #foobar channel to invite-only.

//   :irc.example.com MODE #foobar +o bunny
//                                   ; The irc.example.com server gave channel
//                                   operator privileges to bunny on #foobar.




