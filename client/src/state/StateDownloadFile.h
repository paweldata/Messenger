#ifndef CLIENT_STATEDOWNLOADFILE_H
#define CLIENT_STATEDOWNLOADFILE_H


#include <condition_variable>
#include <fstream>
#include "State.h"

class StateDownloadFile : public State {
public:
    explicit StateDownloadFile(Client *client);
    void execute() override;
    void newMessages() override;

private:
    std::condition_variable cv;
    //std::mutex block;
    bool filenamesExists;
    bool afterDownload;

    std::ofstream file;
    std::string filename;
};


#endif //CLIENT_STATEDOWNLOADFILE_H
