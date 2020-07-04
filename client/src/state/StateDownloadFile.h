#ifndef CLIENT_STATEDOWNLOADFILE_H
#define CLIENT_STATEDOWNLOADFILE_H


#include "State.h"

class StateDownloadFile : public State {
public:
    explicit StateDownloadFile(Client *client) : State(client) {};
    void execute() override;
    void showFiles();
};


#endif //CLIENT_STATEDOWNLOADFILE_H
