#include "Client.h"

Client::Client(int socket) {
    this->socket = socket;
    this->nick = "";
}

void Client::setNick(std::string n) {
    this->nick = n;
}

int Client::getSocket() {
    return this->socket;
}

std::string Client::getNick() const {
    return this->nick;
}

void Client::createFile(std::string name) {
    this->filename = std::move(name);
}

void Client::writeToFile(const std::string& data) {
    this->file.open(this->filename, std::ios_base::app);
    this->file << data;
    this->file.close();
}
