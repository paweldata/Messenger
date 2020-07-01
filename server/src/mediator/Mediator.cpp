#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <algorithm>

#include "Mediator.h"

Mediator::Mediator() {
    this->clientList = std::vector<Client>();
    this->threadList = std::vector<std::thread>();
}

void Mediator::addNewClient(int clientSocket) {
    Client newClient(clientSocket);
    this->clientList.emplace_back(newClient);
    this->threadList.emplace_back(&Mediator::getMessages, this, newClient);
}

void Mediator::getMessages(Client client) {
    this->getNick(client);

    std::string message(1025, 0);
    int size;

    while (true) {
        size = read(client.getSocket(), &message[0], 1024);

        if (size <= 0) {
            close(client.getSocket());
            for (int i = 0; i < this->clientList.size(); i++)
                if (this->clientList[i].getSocket() == client.getSocket()) {
                    this->clientList.erase(this->clientList.begin() + i);
                    return;
                }
        }

        message.insert(0, client.getNick() + ": ");

        for (Client c : this->clientList)
            if (client.getSocket() != c.getSocket())
                send(c.getSocket(), &message[0], size + 12, 0);
    }
}

void Mediator::getNick(Client& client) {
    std::string info = "Give nick (max 10 characters) : ";
    std::string nick(10, ' ');

    send(client.getSocket(), &info[0], info.size(), 0);
    read(client.getSocket(), &nick[0], 10);

    client.setNick(nick);
}
