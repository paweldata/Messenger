#include <sys/socket.h>
#include <iostream>

#include "StateDownloadFile.h"

void StateDownloadFile::execute() {
    send(this->client->getServerSocket(), "fileshow", 8, 0);
}

void StateDownloadFile::showFiles() {
    system("clear");
    std::cout << this->client->getMessages().substr(0, this->client->getMessagesSize()) << "\n";

}
