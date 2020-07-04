#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <thread>
#include <fstream>

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
        if (this->state == WAIT) {
            this->printMessages();
        } else if (this->state == DOWNLOAD_FILE) {
            this->showFiles();
        }

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
    this->state = WAIT;

    while (true) {
        this->printMessages();

        std::cin >> choise;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choise) {
            case '1':  {
                this->state = NEW_MESSAGE;
                this->sendMessage();
                this->state = WAIT;
                break;
            }
            case '2': {
                this->state = UPLOAD_FILE;
                this->uploadFile();
                this->state = WAIT;
                break;
            }
            case '3': {
                this->state = DOWNLOAD_FILE;
                send(this->serverSocket, "fileshow", 8, 0);
                break;
            }
        }
    }
}

void Client::sendMessage() {
    std::string message;
    std::cout << "Message : ";
    std::getline(std::cin, message);

    if (message.size() > 92) {
        std::cout << "Too long message (max 92 characters)\n";
    } else {
        message.insert(0, "message ");
        send(this->serverSocket, &message[0], message.size(), 0);
    }
}

void Client::uploadFile() {
    this->state = UPLOAD_FILE;
    std::cout << "Filename (path to file) : ";
    std::string filename;
    std::cin >> filename;

    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Wrong filename\n";
        sleep(1);
        return;
    }

    std::string data("filename" + filename);
    send(this->serverSocket, &data[0], data.size(), 0);

    char currChar;
    int counter = 8;
    data = std::string(" ", 92);
    data.insert(0, "filedata");

    while (file >> std::noskipws >> currChar) {
        data[counter++] = currChar;

        if (counter >= 100) {
            send(this->serverSocket, &data[0], counter, 0);
            counter = 8;
            data.replace(0, 8, "filedata");
            usleep(100);
        }
    }

    send(this->serverSocket, &data[0], counter, 0);
}

void Client::printMessages() {
    this->block.lock();

    system("clear");
    std::cout << messages.substr(0, this->messagesSize) << "\n";

    if (this->state == WAIT) {
        std::cout << "1 - send message\n"
                     "2 - upload file\n"
                     "3 - download file\n";
    } else if (this->state == DOWNLOAD_FILE) {
        std::cout << "Give filename\n";
    }

    this->block.unlock();
}

void Client::showFiles() {
    std::cout << "Files:\n" << this->messages;
}
