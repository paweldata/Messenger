#ifndef SERVER_CLIENT_H
#define SERVER_CLIENT_H


#include <string>
#include <fstream>

class Client {
public:
    explicit Client(int socket);
    void setNick(std::string n);
    int getSocket();
    std::string getNick() const;

    void createFile(std::string name);
    void writeToFile(const std::string& data);

private:
    int socket;
    std::string nick;
    std::ofstream file;
    std::string filename;
};


#endif //SERVER_CLIENT_H
