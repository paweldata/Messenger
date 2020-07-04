#include <iostream>
#include <sys/socket.h>

#include "NewMessageState.h"
#include "StateMain.h"

void NewMessageState::execute() {
    system("clear");
    std::cout << this->client->getMessages().substr(0, this->client->getMessagesSize()) << "\n";

    std::string message;
    std::cout << "Message : ";
    std::getline(std::cin, message);

    if (message.size() > 92) {
        std::cout << "Too long message (max 92 characters)\n";
    } else {
        message.insert(0, "message ");
        send(this->client->getServerSocket(), &message[0], message.size(), 0);
        this->client->changeState(new StateMain(this->client));
    }
}
