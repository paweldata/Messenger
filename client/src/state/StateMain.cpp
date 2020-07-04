#include <iostream>

#include "StateMain.h"
#include "NewMessageState.h"
#include "StateUploadFile.h"
#include "StateDownloadFile.h"

StateMain::StateMain(Client *client) : State(client) {

}


void StateMain::execute() {
    this->showNewMessages();

    char choise;
    std::cin >> choise;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (choise) {
        case '1':  {
            this->client->changeState(new NewMessageState(this->client));
            break;
        }
        case '2': {
            this->client->changeState(new StateUploadFile(this->client));
            break;
        }
        case '3': {
            this->client->changeState(new StateDownloadFile(this->client));
            break;
        }
    }
}

void StateMain::showNewMessages() {
    this->block.lock();
    system("clear");
    std::cout << this->client->getMessages().substr(0, this->client->getMessagesSize()) << "\n";
    std::cout << "1 - send message\n"
                 "2 - upload file\n"
                 "3 - download file\n";
    this->block.unlock();
}
