// LIST message

//      Command: LIST
//   Parameters: [<channel>{,<channel>}] [<elistcond>{,<elistcond>}]

// The LIST command is used to get a list of channels along with some information about each channel. 
// Both parameters to this command are optional as they have different syntaxes.

// The first possible parameter to this command is a list of channel names, delimited by a comma (",", 0x2C) character. 
// If this parameter is given, the information for only the given channels is returned. If this parameter is not given, 
// the information about all visible channels (those not hidden by the secret channel mode rules) is returned.

// The second possible parameter to this command is a list of conditions as defined in the ELIST RPL_ISUPPORT parameter, 
// delimited by a comma (",", 0x2C) character. Clients MUST NOT submit an ELIST condition unless the server has 
// explicitly defined support for that condition with the ELIST token. If this parameter is supplied, the server filters the returned list of channels with the given conditions as specified in the ELIST documentation.

// In response to a successful LIST command, the server MAY send one RPL_LISTSTART numeric, MUST send back zero or more 
// RPL_LIST numerics, and MUST send back one RPL_LISTEND numeric.

// Numeric Replies:

//     RPL_LISTSTART (321)
//     RPL_LIST (322)
//     RPL_LISTEND (323)

// Command Examples:

//   LIST                            ; Command to list all channels

//   LIST #twilight_zone,#42         ; Command to list the channels
//                                   "#twilight_zone" and "#42".

//   LIST >3                         ; Command to list all channels with
//                                   more than three users.

//   LIST C>60                       ; Command to list all channels with
//                                   created at least 60 minutes ago

//   LIST T<60                       ; Command to list all channels with
//                                   a topic changed within the last 60 minutes

#include "Command.hpp"
#include <iostream>
#include <string>
#include <sstream>

void    Command::List()
{
    std::string message;
	if (checkRegistration() != 0)
    {
        message = ERR_NOTREGISTERED(this->client->getPrefixe(), check_params(this->client->getNickname()));
        send_message(*this->client, message);
        return;
    }
    if (parameters.size() == 0)
    {
        int nbChan = server->getChannelNb();
        for (int i = 0; i < nbChan; i++)
        {
            std::stringstream sstream;
            sstream << server->getChannel(i).getNbClients();
            message = (this->client->getPrefixe()) + " 322 " + (this->client->getNickname()) + " " + server->getChannel(i).getCName();
            message = message + " " + sstream.str() + " :" + server->getChannel(i).getTopic() + "\r\n";
            send_message(*this->client, message);
        }
        std::string message2 = (this->client->getPrefixe()) + " 323 " + (this->client->getNickname()) + " :End of /LIST\r\n";
        send_message(*this->client, message2);
    }

    // autres possiblites non gerees

}

// RPL_LISTSTART (321)
//   "<client> Channel :Users  Name"
// Sent as a reply to the LIST command, this numeric marks the start of a channel list. As noted in the command description, 
// this numeric MAY be skipped by the server so clients MUST NOT depend on receiving it.

// RPL_LIST (322)
//   "<client> <channel> <client count> :<topic>"
// Sent as a reply to the LIST command, this numeric sends information about a channel to the client. <channel> is the 
// name of the channel. <client count> is an integer indicating how many clients are joined to that channel. <topic> is the 
// channel’s topic (as set by the TOPIC command).

// RPL_LISTEND (323)
//   "<client> :End of /LIST"
// Sent as a reply to the LIST command, this numeric indicates the end of a LIST response.
