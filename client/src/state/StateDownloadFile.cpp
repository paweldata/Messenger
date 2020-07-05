#include <sys/socket.h>
#include <iostream>
#include <unistd.h>

#include "StateDownloadFile.h"
#include "StateMain.h"

StateDownloadFile::StateDownloadFile(Client *client) : State(client) {
    this->ready = false;
}

void StateDownloadFile::execute() {
    send(this->client->getServerSocket(), "fileshow", 8, 0);

    std::unique_lock<std::mutex> lck(this->block);
    while (!this->ready)
        this->cv.wait(lck);

    system("clear");
    std::cout << this->client->getMessages().substr(0, this->client->getMessagesSize());

    std::cout << "Choose file : ";
    std::cin >> this->filename;

    std::string data = "filedown" + this->filename;
    send(this->client->getServerSocket(), &data[0], data.size(), 0);
}

void StateDownloadFile::newMessages() {
    if (!this->ready) {
        this->ready = true;
        this->cv.notify_all();
    } else {
        std::string data = this->client->getMessages();
        int datasize = this->client->getMessagesSize();

        if (data.substr(0, 14) == "Wrong filename") {
            std::cout << data << "\n";
            sleep(1);
            this->client->changeState(new StateMain(this->client));
        } else if (data.substr(0, 4) == "file") {
            this->file.open(this->filename, std::ios_base::app);
            this->file << data.substr(8, datasize - 8);
            this->file.close();

            if (data.substr(0, 8) == "fileend ")
                this->client->changeState(new StateMain(this->client));
        }
    }
}
