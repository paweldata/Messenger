#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

#include "Server.h"

Server::Server() {
    struct sockaddr_in serwer =
            {
                    .sin_family = AF_INET,
                    .sin_port = htons(8888)
            };

    struct sockaddr_in address{};
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    socklen_t len = sizeof(serwer);

    if (bind(serverSocket, ( struct sockaddr * ) & serwer, len) < 0) {
        fprintf(stderr, "Bind error");
        exit(-1);
    }

    std::cout << "Server is working\n";

    if (listen(serverSocket, 10) < 0) {
        fprintf(stderr, "Listen error");
        exit(-1);
    }

    while(true) {
        std::cout << "Waiting for connection\n";

        struct sockaddr_in client = { };
        int clientSocket = accept(serverSocket, ( struct sockaddr * ) & client,
                                        reinterpret_cast<socklen_t *>(&len));

        std::cout << "Connect " << client.sin_port <<"\n";
        this->mediator.addNewClient(clientSocket);
    }
}
