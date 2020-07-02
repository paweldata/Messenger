#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <thread>

#include "Client.h"

Client::Client() {
    this->state = WAIT;
    this->messages = std::string(1680, 0);
    this->messagesSize = 0;

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
        if (this->state == WAIT)
            this->printMessages();
    }
}

void Client::chooseOption() {
    // set nick
    this->state = SET_NICK;
    std::string name;
    std::cout << "Give nick (max 10 characters) : ";
    std::getline(std::cin, name);
    send(this->serverSocket, &name[0], name.size(), 0);

    char choise;

    while (true) {
        this->state = WAIT;
        this->printMessages();

        std::cin >> choise;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choise) {
            case '1':  {
                this->state = NEW_MESSAGE;
                this->sendMessage();
                break;
            }
            case '2': {
                this->state = UPLOAD_FILE;
                this->uploadFile();
                break;
            }
        }
    }
}

void Client::sendMessage() {
    std::string message;
    std::getline(std::cin, message);

    std::cout << "Message : ";

    if (message.size() > 100) {
        std::cout << "Too long message (max 100 characters)\n";
    } else {
        std::cout << "Wysylam\n";
        send(this->serverSocket, &message[0], message.size(), 0);
    }
}

void Client::uploadFile() {

}

void Client::printMessages() {
    this->block.lock();

    system("clear");
    std::cout << messages.substr(0, this->messagesSize) << "\n";

    std::cout << "1 - send message\n"
                 "2 - upload file\n"
                 "3 - download file\n";

    this->block.unlock();
}
