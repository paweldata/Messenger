#ifndef CLIENT_STATEMAIN_H
#define CLIENT_STATEMAIN_H


#include <mutex>

#include "State.h"

class StateMain : public State {
public:
    explicit StateMain(Client* client);
    void execute() override;
    void showNewMessages();

private:
    std::mutex block;
};


#endif //CLIENT_STATEMAIN_H
