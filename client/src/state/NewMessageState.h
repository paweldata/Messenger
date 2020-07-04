#ifndef CLIENT_NEWMESSAGESTATE_H
#define CLIENT_NEWMESSAGESTATE_H


#include "State.h"

class NewMessageState : public State {
public:
    explicit NewMessageState(Client* client) : State(client) {};
    void execute() override;
};


#endif //CLIENT_NEWMESSAGESTATE_H
