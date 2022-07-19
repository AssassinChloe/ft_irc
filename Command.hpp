#ifndef COMMANDS_HPP
# define COMMANDS_HPP
# include "ft_irc.hpp"

# define NICK 1
# define JOIN 2
# define USER 3
# define OPER 4
# define QUIT 5
# define CAP 6
# define PING 7
# define MODE 9
# define TOPIC 10
# define LIST 11
# define KICK 12
# define PRIVMSG 13
# define KILL 14
# define PASS 15
# define PART 16
# define WELCOME 17
# define WHO 18
# define NOTICE 19
# define INVITE 20
# define REHASH 21
# define RESTART 22
# define NICKNAME_VALID_CHAR "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_[]{}\\`|"
# define CHANNEL_VALID_CHAR "#&!+"

/*
**  error for command file
*/

# define ERR_NOSUCHNICK(prefixe, nick, arg) (prefixe + "401 " + nick + " " + arg + " :No such nick\r\n") //401
# define ERR_NOSUCHCHANNEL(prefixe, nick, channel) (prefixe + "403 " + nick + " " + channel + " :No such channel\r\n")
 # define ERR_UNKNOWNCOMMAND(command) (command + " :Unknown command\r\n")
# define ERR_NONICKNAMEGIVEN(prefixe) (prefixe + "431 :No nickname given\r\n")
# define ERR_ERRONEUSNICKNAME(nick) (nick + " :Erroneus nickname\r\n")
# define ERR_NICKNAMEINUSE(nick) (nick + " :Nickname is already in use\r\n")
# define ERR_NOTONCHANNEL(channel) (channel + " :You're not on that channel\r\n")
# define ERR_NOTREGISTERED(prefixe, nick) (prefixe + "451 " + nick + " :You have not registered\r\n")
# define ERR_NEEDMOREPARAMS(prefixe, nick, command) (prefixe + "461 " + nick + " " + command + " :Not enough parameters\r\n")
# define ERR_ALREADYREGISTRED(prefixe, nick) (prefixe + "462 " + nick + " :You may not reregister\r\n")
// # define ERR_NOPERMFORHOST() (":Your host isn't among the privileged\r\n")
# define ERR_PASSWDMISMATCH(prefixe, nick) (prefixe + "464 " + nick + " :Password incorrect\r\n")
// # define ERR_CHANNELISFULL(channel) (channel + " :Cannot join channel (+l)\r\n")
# define ERR_UMODEUNKNOWNFLAG(prefixe, nick) (prefixe + "501 " + nick + " :Unknown MODE flag\r\n")
# define ERR_UNKNOWNMODE(prefixe, nick, character) (prefixe + "472 " + nick + " " + character + " :is unknown mode char to me\r\n")
// # define ERR_INVITEONLYCHAN(channel) (channel + " :Cannot join channel (+i)\r\n")
# define ERR_CHANOPRIVSNEEDED(prefixe, nick, channel) (prefixe + "482 " + nick + " " + channel + " :You're not channel operator\r\n")
# define ERR_NOOPERHOST(prefixe, nick) (prefixe + "491 " + nick + " :No O-lines for your host\r\n")
# define ERR_USERSDONTMATCH(prefixe, nick) (prefixe + "502 " + nick + " :Cannot change mode for other users\r\n")
# define ERR_NOTOPIC(channel) (channel + " :No topic is set\r\n")
# define ERR_BADCHANMASK(channel) (channel + " :Bad Channel Mask\r\n") // utilise dans join (mais peut etre commente ici)
// # define ERR_SERVERISFULL(serv_name) (serv_name + " Server is full\r\n")
// # define ERR_KILLDENY(mssg) (":" + mssg + "\r\n")

/*
**  reponse for command file
*/
# define RPL_WELCOME(nickname, prefixe) (prefixe + "001 " + nickname + " :Welcome to the Internet Relay Network " + prefixe + "\r\n")
# define RPL_YOURHOST(prefixe, nickname, servername, version) (prefixe + "002 " + nickname + " :Your host is " + servername + ", running version " + version + "\r\n")
# define RPL_MYINFO(prefixe, nickname, servername, version, usr_modes, chann_modes) (prefixe + "003 " + nickname + " :" + servername + " " + version + " " + usr_modes + " " + chann_modes + "\r\n")
# define RPL_CREATED(prefixe, nickname, date) (prefixe + "004 " + nickname + " :This server was created " + date + "\r\n")


// # define RPL_LISTSTART() ("Channel :Users Name\r\n")
// # define RPL_LIST(channel, topic) (channel + " :" + topic + "\r\n")
// # define RPL_LISTEND() (":End of LIST\r\n")
# define RPL_CHANNELMODEIS(prefixe, nick, channel, mode, params) (prefixe + "324 " + nick + " " + channel + " " + mode + " " + params + "\r\n")
# define RPL_UNIQOPIS(prefixe, nick, channel) (prefixe + "325 " + nick + " " + channel + " " + nick + "\r\n")
# define RPL_NOTOPIC(channel) (channel + " :No topic is set\r\n") // 331 utilise dans topic (mais peut etre commente ici)
# define RPL_TOPIC(channel, subject) (channel + " :" + subject + "\r\n") // 332 utilise dans join et topic (mais peut etre commente ici)
// # define RPL_INVITING(channel, pseudo) (channel + " " + pseudo + "\r\n")
// # define RPL_VERSION(version, debuglevel, server, comment) (version + "." + debuglevel + " " + server + " :" + comment + "\r\n")
# define RPL_WHOREPLY(prefixe, nick, channel, user, host, server, mask, flag, realname) (prefixe + "352 " + nick + " " + channel + " :" + user + " " + host + " " + server + " " + mask + " " + flag + " 0 " + realname + "\r\n")
# define RPL_ENDOFWHO(prefixe, nick, mask) (prefixe + "315 " + nick + " " + mask + " :End of WHO list\r\n")
# define RPL_NAMREPLY(prefixe, nick, channel, nick_list) (prefixe + "353 " + nick + " = " + channel + " :" + nick_list + "\r\n") 
# define RPL_ENDOFNAMES(prefixe, nick, channel) (prefixe + "366 " + nick + " " + channel + " :End of /NAMES list\r\n")
// # define RPL_MOTDSTART(server) (":- " + server + " Message of the day - \r\n")
// # define RPL_MOTD(comment) (":- " + comment + "\r\n")
// # define RPL_ENDOFMOTD() (":End of MOTD command\r\n")
# define RPL_YOUREOPER(prefixe, nickname) (prefixe + "381 " + nickname + " :You are now an IRC operator\r\n")
# define RPL_UMODEIS(prefixe, nick, user_mode) (prefixe + "221 " + nick + " " + user_mode + "\r\n")
// # define RPL_LUSERCLIENT(int1, int2, int3) (":There are " + int1 + " users and " + int2 + " invisible on " + int3 + " servers\r\n")
// # define RPL_LUSEROP(int1) (int1 + " :operator(s) online\r\n")
// # define RPL_LUSERUNKNOWN(int1) (int1 + " :unknown connection(s)\r\n")
// # define RPL_LUSERCHANNELS(int1) (int1 + " :channels formed\r\n")
// # define RPL_ADMINME(server) (server + " :Administrative info\r\n")
// # define RPL_ADMINLOC1(info) (":" + info + "\r\n")
// # define RPL_ADMINLOC2(info) (":" + info + "\r\n")
// # define RPL_ADMINEMAIL(info) (":" + info + "\r\n")
// # define RPL_CREATIONTIME(channel, c_time) (channel + " " + c_time + "\r\n")
// # define RPL_YOURESERVICE(servicename) (":You are service " + servicename + "\r\n")


class Server;
class Client;

void	send_message(Client &cl, std::string message);

class Command
{
	private:
		Client *client;
		Server *server;

		std::string cmdType;
		std::vector<std::string> parameters;
		std::string argLine; 
		bool st;

		std::string getReplies(unsigned short code, std::string arg1, std::string arg2, std::string arg3, std::string arg4, std::string arg5, std::string arg6, std::string arg7);

	public:
		Command(Client &client, Server *ganesh, std::string line);
		~Command();

		Client &getClient();

		std::string getCmdType();
		std::vector<std::string> getParameters();
		std::string getArgLine();

		void	setStop(bool st);
		bool	getStop();
		void	execCommand();
		int		checkRegistration();

		void	Ping();
		// void	Pong(); a supprimer?
		void	nick();
			int isvalidname();

		void	user();
		void	pass();
		void 	Join();
			void names(int index, std::string chanName);
		void	welcome();
		void	Mode();
			int changeChannelMode(std::string modifier, int index);

		void	Who();
		void	Privmsg();
		void	oper();
		void	part();
		void	quit();
		void	Topic();
		void	Notice();
		void	Kick();
		void	List();
		void	Kill();
		void	Invite();
		void	Rehash();
		void	Restart();

};

#endif