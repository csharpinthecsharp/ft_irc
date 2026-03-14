#ifndef SERVER_HPP
#define SERVER_HPP

#include "Utils/Exceptions.hpp"
#include "Utils/Parser.hpp"
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netdb.h>
#define FAIL -1

class Server 
{
    private:
        int _listen_fd;
        unsigned int _loc_port;
    public:
        Server( const Parser& parser );
        ~Server();
        int createSocket();
        void markSocket();
        void acceptCall();

        int getFd() const;
};

#endif