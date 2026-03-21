#include "Client.hpp"

Client::Client( int sock_fd, sockaddr_in client, socklen_t client_size ) 
: _sock_fd(0),
_host(),
_serv(),
_clientSize(0),
_client(),
_authenticated(false),
_registered(false)
{
    this->_sock_fd = sock_fd;
    this->_client = client;
    this->_clientSize = client_size;
    this->fillNameInfo();
}

Client::~Client() 
{
    close(this->_sock_fd);
}


void Client::fillNameInfo() 
{
    int res = getnameinfo((sockaddr*)&_client,
                _clientSize,
                _host,
                NI_MAXHOST,
                _serv, NI_MAXSERV, 0);

    if (res != SUCCESS)
        throw GettingNameInfoFailedException();    
    else
        std::cout << _host << " connect on " << _serv << std::endl;
    return ;
        
}

bool Client::isAuthenticated() const {
    return _authenticated;
}

bool Client::isRegistered() const {
    return _registered;
}

void Client::setAuthenticated(bool value) {
    _authenticated = value;
}

void Client::setRegistered(bool value) {
    _registered = value;
}

void Client::setNick(const std::string& nick) {
    _nick = nick;
}

void Client::setUsername(const std::string& username) {
    _username = username;
}

void Client::setRealname(const std::string& realname) {
    _realname = realname;
}

std::string Client::getNick() const {
    return _nick;
}