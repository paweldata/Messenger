#ifndef CLIENT_STATEUPLOADFILE_H
#define CLIENT_STATEUPLOADFILE_H


#include <fstream>

#include "State.h"

class StateUploadFile : public State {
public:
    explicit StateUploadFile(Client *client) : State(client) {};
    void execute() override;
    void newMessages() override {};

private:
    std::ifstream file;
    std::string filename;
};


#endif //CLIENT_STATEUPLOADFILE_H
