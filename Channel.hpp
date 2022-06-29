#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "ft_irc.hpp"

class Client;

class Server;

class Channel
{
	private:
		std::string name;
		std::string topic;
		std::map<int, Client *> clients;
		std::string mode;
		std::map<int, std::string> client_mode; // voir si pas plutot client, std::string
		// std::string key;
		unsigned int max_clients;


	public:
		Channel (std::string name); //(Server *server, std::string name);
		~Channel();
		Channel(const Channel &src);
		Channel &operator=(Channel const &rhs);

		// void setName(std::string name);
		std::string getCName();

		void setTopic(std::string topic);
		std::string getTopic();

		void addClient(Client &client);
		void removeClient(Client &client);
		void removeClient(std::string const &nick);
		std::vector<Client *> getClients();
		bool isClient(Client &client);
		bool isOnChannel(std::string const &nick);

		void setMode(std::string);
		std::string getMode();

		void setClientMode(Client &client, std::string mode);
		std::string getClientMode(Client &client);
		std::map<int, Client*> getClientMap();

		// void setKey(std::string key);
		// std::string getKey();

		void setMaxClients(int max);
		int getMaxClients();
		int getNbClients(); 
		int formatName(std::string name);

		void broadcast(Client &client, std::string message);

};
#endif