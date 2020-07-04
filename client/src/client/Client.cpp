#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <thread>
#include <fstream>

#include "Client.h"
#include "../state/StateMain.h"
#include "../state/StateDownloadFile.h"

Client::Client() {
    this->stateold = WAIT;
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

    this->readThread = std::thread(&Client::getMessageFromServer, this);
    this->sendThread = std::thread(&Client::chooseOption, this);

    this->readThread.join();
    this->sendThread.join();
}

void Client::getMessageFromServer() {
    while (true) {
        this->messagesSize = read(this->serverSocket, &this->messages[0], 1680);
        if (auto* currState = dynamic_cast<StateMain*>(this->state)) {
            currState->showNewMessages();
        } else if (auto* currState = dynamic_cast<StateDownloadFile*>(this->state)) {
            currState->showFiles();
        }
    }
}

void Client::chooseOption() {
    // set nick
    this->stateold = SET_NICK;
    std::string name;
    std::cout << "Give nick (max 10 characters) : ";
    std::getline(std::cin, name);
    send(this->serverSocket, &name[0], name.size(), 0);

    while (true)
        this->state->execute();
}

void Client::showFiles() {
    std::cout << "Files:\n" << this->messages;
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
