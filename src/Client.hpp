#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"
#include "unistd.h"
#include <iostream>

class Client {
    private:
        int _loc_listen_fd;
        char _host[NI_MAXHOST];
        char _serv[NI_MAXSERV];
        socklen_t _clientSize;
        sockaddr_in _client;
    public:
        Client( const Server& server );
        ~Client();

        void acceptCall();
        void fillNameInfo();
};

#endif