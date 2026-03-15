#include "Server.hpp"

Server::Server( const Parser& parser ) 
: _listen_fd(0),
_clients()
{
    this->_loc_port = parser.getPort();
    this->_listen_fd = this->createSocket();
    this->markSocket();
}
Server::~Server() {}

int Server::createSocket() const 
{
    // AF_INET = IPV4 (domain);
    // SOCK_STREAM = Connection-oriented communication (type);
    // IPPROTO_TCP = PROTOCOL (TCP);
    int temp = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (temp == FAIL)
        throw ServerSocketCreationFailedException();
    return temp;
}

void Server::markSocket() const
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

void Server::open() {
    while (1) 
    {
        sockaddr_in client;
        socklen_t clientSize = sizeof(client);

        int clientSocket = accept(this->_listen_fd,
                                (sockaddr*)&client,
                                &clientSize);
        if (clientSocket == FAIL)
            throw ClientSocketCreationFailedException();

        _clients.push_back(Client(clientSocket, client, clientSize));

        // Maintenant je dois répondre à (17:02 Waiting for CAP LS response...)
    }
    //close(this->_listen_fd);
}


