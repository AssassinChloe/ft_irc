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
