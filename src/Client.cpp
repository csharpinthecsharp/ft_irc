#include "Client.hpp"

Client::Client() {
}

Client::Client( int sock_fd, sockaddr_in client, socklen_t client_size ) 
: _sock_fd(0),
_host(),
_serv(),
_clientSize(0),
_client(),
_authenticated(false),
_registered(false),
_buffer("")
{
    this->_sock_fd = sock_fd;
    this->_client = client;
    this->_clientSize = client_size;
    this->fillNameInfo();
}

Client::~Client() 
{
}


void Client::fillNameInfo() 
{
    int res = getnameinfo((sockaddr*)&this->_client,
                _clientSize,
                _host,
                NI_MAXHOST,
                _serv, NI_MAXSERV, 0);

    if (res != SUCCESS)
        throw GettingNameInfoFailedException();    
    else
        std::cout << this->_host << " connect on " << this->_serv << std::endl;
    return ;
        
}

void Client::appendBuffer( const std::string& buffer )
{
    this->_buffer.append(buffer);
}

void Client::handleBufferData()
{
    std::cout << "My buffer is containing: " << _buffer << std::endl;
} 


bool Client::isAuthenticated() const {
    return this->_authenticated;
}

bool Client::isRegistered() const {
    return this->_registered;
}

void Client::setAuthenticated(bool value) {
    this->_authenticated = value;
}

void Client::setRegistered(bool value) {
    this->_registered = value;
}

void Client::setNick(const std::string& nick) {
    this->_nick = nick;
}

void Client::setUsername(const std::string& username) {
    this->_username = username;
}

void Client::setRealname(const std::string& realname) {
    this->_realname = realname;
}

const std::string& Client::getNick() const {
    return this->_nick;
}

void Client::sendReply(const std::string& reply) { (void)reply; return ; }
