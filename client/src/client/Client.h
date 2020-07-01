#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H


#include <thread>

class Client {
public:
    Client();

private:
    void getMessage();
    void sendMessage();

    int serverSocket;
    std::thread readThread;
    std::thread sendThread;
};


#endif //CLIENT_CLIENT_H
