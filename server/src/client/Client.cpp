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

std::string Client::getNick() {
    return this->nick;
}
