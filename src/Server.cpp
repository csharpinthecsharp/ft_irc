#include "Server.hpp"

Server::Server() : _listen_fd(0)
{
    this->_listen_fd = this->createSocket();
}
Server::~Server() {}

int Server::createSocket() 
{
    // AF_INET = IPV4 (domain);
    // SOCK_STREAM = Connection-oriented communication (type);
    // IPPROTO_TCP = PROTOCOL (TCP);
    int temp = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (temp == -1)
        throw SocketCreationFailedException();
    return temp;
}