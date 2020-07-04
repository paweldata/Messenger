#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <mutex>

#include "Mediator.h"

int Mediator::messagesMaxAmount = 15;

Mediator::Mediator() {
    this->clientList = std::vector<Client*>();
    this->threadList = std::vector<std::thread>();
    this->messages = "";
    this->messagesCounter = 0;
}

void Mediator::addNewClient(int clientSocket) {
    Client* newClient = new Client(clientSocket);
    this->threadList.emplace_back(&Mediator::getMessages, this, newClient);
}

void Mediator::getMessages(Client* client) {
    this->getNick(client);
    this->clientList.emplace_back(client);

    send(client->getSocket(), &this->messages[0], this->messages.size(), 0);

    std::string message(112, 0);
    int size;

    while (true) {
        size = read(client->getSocket(), &message[0], 112);

        if (size <= 0) {
            // close connection
            close(client->getSocket());
            for (int i = 0; i < this->clientList.size(); i++)
                if (this->clientList[i]->getSocket() == client->getSocket()) {
                    this->clientList.erase(this->clientList.begin() + i);
                    return;
                }
        }

        if (message.substr(0, 8) == "message ") {
            this->getMessage(message, client, size);
        } else if (message.substr(0, 4) == "file") {
            this->getFile(message, client, size);
        }
    }
}

void Mediator::getNick(Client* client) {
    std::string nick(10, ' ');
    read(client->getSocket(), &nick[0], 10);

    client->setNick(nick);
}

void Mediator::getMessage(std::string message, Client* client, int size) {
    if (this->messagesCounter <= Mediator::messagesMaxAmount)
        this->messagesCounter++;

    message.erase(0, 8);
    message.insert(0, client->getNick() + ": ");
    std::size_t found = this->messages.find('\n');

    if (found != std::string::npos && this->messagesCounter > Mediator::messagesMaxAmount)
        this->messages = this->messages.substr(found + 1);
    this->messages.append(message.substr(0, size + 4) + '\n');

    for (Client* c : this->clientList)
        send(c->getSocket(), &this->messages[0], this->messages.size(), 0);
}

void Mediator::getFile(const std::string& message, Client* client, int size) {
    if (message.substr(0, 8) == "filename") {
        client->createFile("files/" + message.substr(8, size - 8));
    } else if (message.substr(0, 8) == "filedata") {
        client->writeToFile(message.substr(8, size - 8));
    }
}
