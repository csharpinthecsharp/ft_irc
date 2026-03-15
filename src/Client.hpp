#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"
#include "unistd.h"
#include <iostream>

class Client {
    private:
        int _sock_fd;
        char _host[NI_MAXHOST];
        char _serv[NI_MAXSERV];
        socklen_t _clientSize;
        sockaddr_in _client;
    public:
        Client( int sock_fd, sockaddr_in client, socklen_t client_size );
        ~Client();

        void fillNameInfo();
};

#endif