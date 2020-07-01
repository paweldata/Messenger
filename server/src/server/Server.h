#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H


#include "../mediator/Mediator.h"

#include <string>

class Server {
public:
    Server();

private:
    Mediator mediator;
};


#endif //SERVER_SERVER_H
