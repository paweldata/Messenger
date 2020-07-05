#include <iostream>

#include "StateMain.h"
#include "StateNewMessage.h"
#include "StateUploadFile.h"
#include "StateDownloadFile.h"

void StateMain::execute() {
    this->newMessages();

    char choise;
    std::cin >> choise;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (choise) {
        case '1':  {
            this->client->changeState(new StateNewMessage(this->client));
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
        case '4': {
            exit(0);
        }
    }
}

void StateMain::newMessages() {
    this->block.lock();
    system("clear");
    std::cout << this->client->getMessages().substr(0, this->client->getMessagesSize()) << "\n";
    std::cout << "1 - send message\n"
                 "2 - upload file\n"
                 "3 - download file\n"
                 "4 - exit\n";
    this->block.unlock();
}
