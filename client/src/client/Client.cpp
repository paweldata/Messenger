#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <thread>

#include "Client.h"
#include "../state/StateMain.h"

Client::Client() {
    this->messages = std::string(1680, 0);
    this->messagesSize = 0;
    this->state = new StateMain(this);

    struct sockaddr_in serwer =
            {
                    .sin_family = AF_INET,
                    .sin_port = htons(8888)
            };

    this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    socklen_t len = sizeof( serwer );

    if (connect(this->serverSocket, (struct sockaddr *)&serwer, len) < 0) {
        fprintf(stderr, "Connect error\n");
        exit(-1);
    }

    // set nick
    std::string name;
    std::cout << "Give nick (max 8 characters) : ";
    std::getline(std::cin, name);
    send(this->serverSocket, &name[0], std::min(int(name.size()), 8), 0);

    this->readThread = std::thread(&Client::getMessageFromServer, this);
    this->sendThread = std::thread(&Client::getMessageFromClient, this);

    this->readThread.join();
    this->sendThread.join();
}

void Client::getMessageFromServer() {
    while (true) {
        this->messagesSize = read(this->serverSocket, &this->messages[0], 1680);
        this->state->newMessages();
    }
}

void Client::getMessageFromClient() {
    while (true)
        this->state->execute();
}

std::string Client::getMessages() {
    return this->messages;
}

int Client::getMessagesSize() {
    return this->messagesSize;
}

int Client::getServerSocket() {
    return this->serverSocket;
}

void Client::changeState(State *state) {
    this->state = state;
}
