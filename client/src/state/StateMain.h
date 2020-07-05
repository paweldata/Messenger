#ifndef CLIENT_STATEMAIN_H
#define CLIENT_STATEMAIN_H


#include <mutex>

#include "State.h"

class StateMain : public State {
public:
    explicit StateMain(Client* client) : State(client) {};
    void execute() override;
    void newMessages() override;

private:
    std::mutex block;
};


#endif //CLIENT_STATEMAIN_H
