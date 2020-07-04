#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H


#include <thread>
#include <mutex>

#include "ClientState.cpp"

class Client {
public:
    Client();

private:
    void getMessageFromServer();
    void chooseOption();

    void sendMessage();
    void uploadFile();

    void showFiles();

    void printMessages();

    int serverSocket;
    std::thread readThread;
    std::thread sendThread;

    std::string messages;
    int messagesSize;
    ClientState state;

    std::mutex block;
};


#endif //CLIENT_CLIENT_H
