#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <thread>

#include "Client.h"

Client::Client() {
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

    std::cout << "Connect to server\n";

    this->readThread = std::thread(&Client::getMessage, this);
    this->sendThread = std::thread(&Client::sendMessage, this);

    this->readThread.join();
    this->sendThread.join();
}

void Client::getMessage() {
    std::string message(1025, 0);

    while (true) {
        int size = read(this->serverSocket, &message[0], 1024);
        std::cout << message.substr(0, size) << "\n";
    }
}

void Client::sendMessage() {
    std::string message;

    while (true) {
        std::getline(std::cin, message);
        send(this->serverSocket, &message[0], message.size(), 0);
    }
}
