#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

#include "Client.h"

Client::Client() {
    struct sockaddr_in serwer =
            {
                    .sin_family = AF_INET,
                    .sin_port = htons(8888)
            };

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    socklen_t len = sizeof( serwer );

    if (connect(serverSocket, (struct sockaddr *)&serwer, len) < 0) {
        fprintf(stderr, "Connect error");
        exit(-1);
    }

    std::cout << "Connect to server\n";
}
