#ifndef SERVER_MEDIATOR_H
#define SERVER_MEDIATOR_H


#include "../client/Client.h"

#include <vector>
#include <string>
#include <thread>

class Mediator {
public:
    Mediator();
    void addNewClient(int clientSocket);
    void getMessages(Client* client);
    void getNick(Client* client);

private:
    void getMessage(std::string message, Client* client, int size);
    void fileOptions(const std::string& message, Client* client, int size);

    std::vector<Client*> clientList;
    std::vector<std::thread> threadList;
    std::string messages;

    const int messagesMaxAmount = 15;
    int messagesCounter;
};


#endif //SERVER_MEDIATOR_H
