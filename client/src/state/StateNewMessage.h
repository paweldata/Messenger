#ifndef CLIENT_STATENEWMESSAGE_H
#define CLIENT_STATENEWMESSAGE_H


#include "State.h"

class StateNewMessage : public State {
public:
    explicit StateNewMessage(Client* client) : State(client) {};
    void execute() override;
    void newMessages() override {};
};


#endif //CLIENT_STATENEWMESSAGE_H
