#ifndef SERVER_CLIENT_H
#define SERVER_CLIENT_H


#include <string>

class Client {
public:
    explicit Client(int socket);
    void setNick(std::string n);
    int getSocket();
    std::string getNick();

private:
    int socket;
    std::string nick;
};


#endif //SERVER_CLIENT_H
