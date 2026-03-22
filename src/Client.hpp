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
        bool _authenticated;
        bool _registered;
        std::string _nick;
        std::string _username;
        std::string _realname;

        std::string _buffer; // Le buffer client n'a pas de limite
        // IIl doit pouvoir contenir plusieurs messages a la fois.
        // La delimiation pour chaque message est \r\n.
    public:
        Client();
        Client( int sock_fd, sockaddr_in client, socklen_t client_size );
        ~Client();

        void appendBuffer( const std::string& buffer );
        void handleBufferData();
        void handleCAPLS();
        void handleCAPEND();
        void handleUserInfos();

        void fillNameInfo();
        bool isAuthenticated() const;
        bool isRegistered() const;
        void setAuthenticated(bool value);
        void setRegistered(bool value);
        void sendReply(const std::string& reply);
        void setNick(const std::string& nick);
        void setUsername(const std::string& username);
        void setRealname(const std::string& realname);
        const std::string& getNick() const;
};

#endif