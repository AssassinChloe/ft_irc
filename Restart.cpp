#include "Command.hpp"


void    Command::Restart()
{
    std::string clientStat = this->client->getStatus();
    if (searchIfMode('o', clientStat) != 1) // si client n'est pas operateur
    {
        std::string message = this->client->getPrefixe() + " 481 " + this->client->getNickname() + " :Permission Denied- You're not an IRC operator\r\n";
        send_message(*this->client, message);
        return ;
    }
    
    this->server->cleanClose();
    // close(this->server->getSocketFd());
 
    // std::vector<int> FDs;
    // int i = 0;
    // for (std::map<int, Client>::iterator it = this->server->getClientList().begin(); it != this->server->getClientList().end(); it++)
    //         {
    //             std::cout << "it.first " << (*it).first <<std::endl;
    //             FDs.push_back(it->first);
    //             std::cout << "FD[i] " << FDs[i] <<std::endl;
    //             i++;
    //             std::cout <<"1 client to delete" <<std::endl;
    //         }
    // std::cout << "fin de la boucle 1" <<std::endl;

    // for (int j = 0; j<i; j++)
    // {
    //     this->server->deleteClient(FDs[j]);
    //     // std::cout <<"1 client deleted" <<std::endl;
    // }

    // g_stop = 0;
	// signal(SIGINT, sighandler);
    // signal(SIGPIPE, sighandler); 
	if (this->server->init() < 0)
		return;
	while (g_stop == 0)
    {
        this->server->run();
	}
    return ;

}