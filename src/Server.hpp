#ifndef SERVER_HPP
#define SERVER_HPP

#include "Utils/Exceptions.hpp"
#include <sys/socket.h>
#include <netinet/ip.h>

#define FAIL -1

class Server 
{
    private:
        int _listen_fd;
    public:
        Server();
        ~Server();
        int createSocket();
};

#endif