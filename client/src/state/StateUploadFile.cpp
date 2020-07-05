#include <iostream>
#include <unistd.h>
#include <sys/socket.h>

#include "StateUploadFile.h"
#include "StateMain.h"

void StateUploadFile::execute() {
    system("clear");
    std::cout << this->client->getMessages().substr(0, this->client->getMessagesSize()) << "\n";
    std::cout << "Filename (path to file) : ";
    std::cin >> this->filename;

    this->file = std::ifstream(filename);
    if (!file.is_open()) {
        std::cout << "Wrong filename\n";
        this->client->changeState(new StateMain(this->client));
        return;
    }

    std::string data("filename" + this->filename);
    send(this->client->getServerSocket(), &data[0], data.size(), 0);

    std::cout << "Sending file " << this->filename << "\n";

    char currChar;
    int counter = 8;
    data = std::string(" ", 92);
    data.insert(0, "filedata");

    while (this->file >> std::noskipws >> currChar) {
        data[counter++] = currChar;

        if (counter >= 100) {
            send(this->client->getServerSocket(), &data[0], counter, 0);
            counter = 8;
            usleep(100);
        }
    }

    send(this->client->getServerSocket(), &data[0], counter, 0);
    this->client->changeState(new StateMain(this->client));
}
