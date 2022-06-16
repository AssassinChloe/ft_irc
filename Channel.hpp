#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <map>
# include <vector>

class Client;

class Channel
{
	private:
		std::string name;
		std::string topic;
		std::map<int, Client *> clients;
		std::string mode;
		std::map<int, std::string> client_mode;
		std::string key;
		std::string max_clients;
		std::vector<Client *> invited;

	public:
		Channel();

		void setName(std::string name);
		std::string getName();

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

		void setKey(std::string key);
		std::string getKey();

		void setMaxClients(std::string max);
		std::string getMaxClients();

		void addInvited(Client &client);
		bool isInvited(Client &client);
		void removeInvited(Client &client);

		void broadcast(Client &client, std::string message);

};
#endif