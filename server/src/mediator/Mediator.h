#ifndef SERVER_MEDIATOR_H
#define SERVER_MEDIATOR_H


#include <vector>
#include <string>
#include <thread>

class Mediator {
public:
    Mediator();
    void addNewClient(int clientSocket);
    void getMessages(int clientSocket);

private:
    std::vector<int> clientSocketList;
    std::vector<std::thread> threadList;
};


#endif //SERVER_MEDIATOR_H
