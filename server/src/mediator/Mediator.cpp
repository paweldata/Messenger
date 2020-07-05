#include <sys/socket.h>
#include <unistd.h>
#include <dirent.h>
#include <algorithm>
#include <cstring>
#include <mutex>
#include <iostream>

#include "Mediator.h"

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
                    std::string lastMessage(this->clientList[i]->getNick() + "[disconnect]");
                    this->getMessage(lastMessage, this->clientList[i], lastMessage.size());
                    this->clientList.erase(this->clientList.begin() + i);
                    return;
                }
        }

        if (message.substr(0, 8) == "message ") {
            this->getMessage(message, client, size);
        } else if (message.substr(0, 4) == "file") {
            this->fileOptions(message, client, size);
        }
    }
}

void Mediator::getNick(Client* client) {
    std::string nick(10, ' ');
    read(client->getSocket(), &nick[0], 10);

    client->setNick(nick);

    std::string newMessage = nick + "[connect to server]";
    this->getMessage(newMessage, client, newMessage.size());
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

void Mediator::fileOptions(const std::string& message, Client* client, int size) {
    if (message.substr(0, 8) == "filename") {
        client->createFile("files/" + message.substr(8, size - 8));
    } else if (message.substr(0, 8) == "filedata") {
        client->writeToFile(message.substr(8, size - 8));
    } else if (message.substr(0, 8) == "fileend ") {
        client->writeToFile(message.substr(8, size - 8));
        std::string newMessage(client->getNick() + "[upload new file]");
        this->getMessage(newMessage, client, newMessage.size());
    } else if (message.substr(0, 8) == "fileshow") {
            DIR* dir;
            struct dirent *de = nullptr;
            std::string files;

            if ((dir = opendir ("./files/")) != nullptr) {
                while ((de = readdir(dir)) != nullptr) {
                    if (strcmp(de->d_name, ".") != 0  && strcmp(de->d_name, "..") != 0) {
                        files.append(de->d_name);
                        files.append("\n");
                    }
                }
                closedir (dir);
                send(client->getSocket(), &files[0], files.size(), 0);
            } else {
                send(client->getSocket(), "Server error\n", 12, 0);
            }
    } else if (message.substr(0, 8) == "filedown") {
        std::ifstream file("files/" + message.substr(8, size - 8));

        if (!file.is_open()) {
            send(client->getSocket(), "Wrong filename\n", 14, 0);
            sleep(1);
            send(client->getSocket(), &this->messages[0], this->messages.size(), 0);
            return;
        }

        char currChar;
        int counter = 8;
        std::string data = std::string(" ", 92);
        data.insert(0, "filedata");

        while (file >> std::noskipws >> currChar) {
            data[counter++] = currChar;

            if (counter >= 100) {
                send(client->getSocket(), &data[0], counter, 0);
                counter = 8;
                usleep(100);
            }
        }

        data.replace(0, 8, "fileend ");
        send(client->getSocket(), &data[0], counter, 0);
        usleep(100);
        send(client->getSocket(), &this->messages[0], this->messages.size(), 0);
    }
}
