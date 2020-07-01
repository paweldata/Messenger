#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

#include "Mediator.h"

Mediator::Mediator() {
    this->clientSocketList = std::vector<int>();
    this->threadList = std::vector<std::thread>();
}

void Mediator::addNewClient(int clientSocket) {
    this->clientSocketList.emplace_back(clientSocket);
    this->threadList.emplace_back(&Mediator::getMessages, this, clientSocket);
}

void Mediator::getMessages(int clientSocket) {
    std::string message(1025, 0);
    int size;

    while (true) {
        size = read(clientSocket, &message[0], 1024);

        if (size <= 0) {
            std::cout << "Disconnect\n";
            close(clientSocket);
            break;
        }

        for (int socket : this->clientSocketList)
            if (socket != clientSocket)
                send(socket, &message[0], size, 0);
    }
}
