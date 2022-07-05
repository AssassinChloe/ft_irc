
#include "Command.hpp"

void    Command::Kill()
{
        
    int nb_param = parameters.size();

    if (nb_param <= 1) 
    {
        std::string message = parameters[0] + " :Not enough parameters\r\n"; // ERR_NEEDMOREPARAMS (461)
        send_message(*this->client, message);
        return;
    }
    
    else
    {
        // check if client is operator
        bool hasPower = 1;
        if (!hasPower)
        {
            std::string message = this->client->getPrefixe() + " 481 " + this->client->getNickname() + " :Permission Denied- You're not an IRC operator\r\n";
            send_message(*this->client, message);
            // :admin!admin@localhost 481 admin :Permission Denied- You're not an IRC operator
        }
        else
        {
            // si on trouve pas l'utlisateur
            // err no such nick
            // sinon
        }
    }
}


//     KILL message

//      Command: KILL
//   Parameters: <nickname> <comment>

// The KILL command is used to close the connection between a given client and the server they are connected to. 
// KILL is a privileged command and is available only to IRC Operators. <nickname> represents the user to be ‘killed’, 
// and <comment> is shown to all users and to the user themselves upon being killed.

// When a KILL command is used, the client being killed receives the KILL message, and the <source> of the message 
// SHOULD be the operator who performed the command. The user being killed and every user sharing a channel with 
// them receives a QUIT message representing that they are leaving the network. The <reason> on this QUIT message 
// typically has the form: "Killed (<killer> (<reason>))" where <killer> is the nickname of the user who performed 
// the KILL. The user being killed then receives the ERROR message, typically containing a <reason> of 
// "Closing Link: <servername> (Killed (<killer> (<reason>)))". After this, their connection is closed.

// If a KILL message is received by a client, it means that the user specified by <nickname> is being killed. With 
// certain servers, users may elect to receive KILL messages created for other users to keep an eye on the network. 
// This behavior may also be restricted to operators.

// Clients can rejoin instantly after this command is performed on them. However, it can serve as a warning to a 
// user to stop their activity. As it breaks the flow of data from the user, it can also be used to stop large amounts 
// of ‘flooding’ from abusive users or due to accidents. Abusive users may not care and promptly reconnect and resume 
// their abusive behaviour. In these cases, opers may look at the KLINE command to keep them from rejoining the network 
// for a longer time.

// As nicknames across an IRC network MUST be unique, if duplicates are found when servers join, one or both of the 
// clients MAY be KILLed and removed from the network. Servers may also handle this case in alternate ways that don’t 
// involve removing users from the network.

// Servers MAY restrict whether specific operators can remove users on other servers (remote users). If the operator 
// tries to remove a remote user but is not privileged to, they should receive the ERR_NOPRIVS (723) numeric.

// <comment> SHOULD reflect why the KILL was performed. For user-generated KILLs, it is up to the user to provide an 
// adequate reason.

// Numeric Replies:

//     ERR_NOSUCHSERVER (402)
//     ERR_NEEDMOREPARAMS (461)
//     ERR_NOPRIVILEGES (481)
//     ERR_NOPRIVS (723)

// ERR_NOPRIVILEGES (481)
//   "<client> :Permission Denied- You're not an IRC operator"
// Indicates that the command failed because the user is not an IRC operator. The text used in the last param of this 
// message may vary.


// 6 < KILL lilin :il est pas beau
// 6 > :admin!admin@localhost 481 admin :Permission Denied- You're not an IRC operator
// 6 < OPER admin admin
// 6 > :admin!admin@localhost 381 admin :You are now an IRC operator
// 6 > :admin!admin@localhost 221 admin +wo
// 6 < KILL lilin :il ne me plait pas
// 4 > :admin!admin@localhost KILL :il ne me plait pas
// 4 > :lilin!liliu@localhost QUIT :il ne me plait pas
// 5 > :lilin!liliu@localhost QUIT :il ne me plait pas
// 6 > :lilin!liliu@localhost QUIT :il ne me plait pas