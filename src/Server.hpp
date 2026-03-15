#ifndef SERVER_HPP
#define SERVER_HPP

#include "Utils/Exceptions.hpp"
#include "Utils/Parser.hpp"
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <vector>
#include "Client.hpp"
#define FAIL -1
#define SUCCESS 0

class Client;

class Server 
{
    private:
        int _listen_fd;
        unsigned int _loc_port;
        std::vector<Client> _clients;
    public:
        Server( const Parser& parser );
        ~Server();
        int createSocket() const;
        void markSocket() const;

        void open();
};

#endif