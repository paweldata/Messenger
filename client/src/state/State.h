#ifndef CLIENT_STATE_H
#define CLIENT_STATE_H


#include "../client/Client.h"

class Client;

class State {
public:
    State(Client* client);
    virtual void execute() = 0;
    virtual void newMessages() = 0;

protected:
    Client* client;
};


#endif //CLIENT_STATE_H
