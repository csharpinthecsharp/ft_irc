#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"
#include "Message.hpp"
#include "Channel/Channel.hpp"
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
        std::string _serverPassword;

        std::string _buffer;

    public:
        Client();
        Client(int sock_fd, sockaddr_in client, socklen_t client_size, const std::string& password);
        ~Client();

        void shutdown();
        void tryToRegister();
        void leaveChannel( Channel& channel );

        void fillNameInfo();
        void appendBuffer( const std::string& buffer );
        int getSockFd() const;
        
        bool isAuthenticated() const;
        bool isRegistered() const;
        void setAuthenticated(bool value);
        void setRegistered(bool value);
        void sendReply(const std::string& reply);
        void handleBufferData();
        void eraseBuffer(size_t len);

        void setNick(const std::string& nick);
        void setUsername(const std::string& username);
        void setRealname(const std::string& realname);
        const std::string& getNick() const;
        const std::string& getUsername() const;
        const std::string& getRealname() const;
        const std::string& getBuffer() const;
        const char* getHost() const;
};

std::ostream& operator<<( std::ostream& os, const Client& other );

#endif