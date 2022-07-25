/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmercier <vmercier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 11:37:03 by vmercier          #+#    #+#             */
/*   Updated: 2022/07/25 13:41:48 by vmercier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "ft_irc.hpp"

class Client;

class Server;

class Channel
{
	private:
		std::string					name;
		std::string 				topic;
		std::map<int, Client *>		clients;
		std::string 				mode;
		std::map<int, std::string>	client_mode;
		unsigned int 				max_clients;
		Client		 				*topicSetter;
		time_t          			lastTopicSet;
		std::vector<std::string> 	invited;
		Channel (); 

	public:
		Channel (std::string name); 
		~Channel();
		Channel(const Channel &src);
		Channel &operator=(Channel const &rhs);

		int formatName(std::string name);
		std::string getCName();

		void setTopic(std::string topic);
		std::string getTopic();

		int getNbClients(); 
		void addClient(Client &client);
		void removeClient(Client &client);
		void removeClient(std::string const &nick);
		std::vector<Client *> getClients();
		bool isClient(Client &client); // non utilise
		bool isOnChannel(std::string const &nick);

		void setMode(std::string);
		std::string getMode();
		void addMode(char mode);
		void delMode(char mode);

		void setClientMode(Client &client, std::string mode); // non utilise
		std::string getClientMode(Client &client);
		std::map<int, Client*> getClientMap();

		Client 	*getTopicSetter();
		void 	setTopicSetter(Client *client);
		time_t  getLastTopicSet();
		void    setLastTopicSet();

		void	broadcast(std::string message);

		void addInvited(std::string nickname);
        void delInvited(std::string nickname);
        std::string getInvited(int i);
        int getInvitedIndex(std::string nickname);
        int getInvitedNb();

};
#endif