#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H


#include <thread>
#include <mutex>

#include "../state/State.h"

class State;

class Client {
public:
    Client();
    std::string getMessages();
    int getMessagesSize();
    int getServerSocket();
    void changeState(State* state);

private:
    void getMessageFromServer();
    void chooseOption();

    int serverSocket;
    std::thread readThread;
    std::thread sendThread;

    std::string messages;
    int messagesSize;

    State* state;
};


#endif //CLIENT_CLIENT_H
