#ifndef SERVER_HPP
#define SERVER_HPP

#include "Utils/Exceptions.hpp"
#include "Utils/Parser.hpp"
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <map>
#include "Client.hpp"
#include "Message.hpp"
#include <poll.h>
#include <vector>
#include "Commands/Commands.hpp"
#include "Channel/Channel.hpp"
#define FAIL -1
#define SUCCESS 0

class Client;

class Server 
{
    private:
        int _listen_fd;
        unsigned int _loc_port;
        std::string _password;
        std::map<int, Client> _clients;
        std::map<std::string, Channel> _channels;
    public:
        Server( const Parser& parser );
        ~Server();
        int createSocket() const;
        void markSocket() const;

        void open();
        bool dispatch(const Message& msg, Client& client);
};

#endif