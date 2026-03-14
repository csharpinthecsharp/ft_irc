#include "Server.hpp"

Server::Server( const Parser& parser ) : _listen_fd(0)
{
    this->_loc_port = parser.getPort();
    this->_listen_fd = this->createSocket();
    this->markSocket();
}
Server::~Server() {}

int Server::createSocket() 
{
    // AF_INET = IPV4 (domain);
    // SOCK_STREAM = Connection-oriented communication (type);
    // IPPROTO_TCP = PROTOCOL (TCP);
    int temp = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (temp == FAIL)
        throw ServerSocketCreationFailedException();
    return temp;
}

void Server::markSocket()
{
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(this->_loc_port); // htons > flip bits 
    hint.sin_addr.s_addr = INADDR_ANY;

    // Bind PORT/IP
    if (bind(this->_listen_fd, (sockaddr*)&hint, sizeof(hint)) == FAIL)
        throw BindFailedExpection();

    // Mark for listening
    if (listen(this->_listen_fd, SOMAXCONN) == FAIL)
        throw ListenFailedExpection();
    return ;
}

int Server::getFd() const {
    return this->_listen_fd;
}


