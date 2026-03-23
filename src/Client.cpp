#include "Client.hpp"
#include "Commands/Commands.hpp"

Client::Client() {}

Client::Client( int sock_fd, sockaddr_in client, socklen_t client_size, const std::string& password) 
: _sock_fd(0),
_host(),
_serv(),
_clientSize(0),
_client(),
_authenticated(false),
_registered(false),
_nick(""),
_username(""),
_realname(""),
_serverPassword(password),
_buffer("")
{
    this->_sock_fd = sock_fd;
    this->_client = client;
    this->_clientSize = client_size;
    this->fillNameInfo();
}

Client::~Client() {}


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
        std::cout << "🔎 " << this->_host << " is now connected to " << this->_serv << std::endl;
    return ;
        
}

void Client::appendBuffer( const std::string& buffer )
{
    this->_buffer.append(buffer);
}

bool Client::isAuthenticated() const 
{
    return (this->_authenticated);
}

bool Client::isRegistered() const 
{
    return (this->_registered);
}

void Client::setAuthenticated(bool value) 
{
    this->_authenticated = value;
}

void Client::setRegistered(bool value) 
{
    this->_registered = value;
}

void Client::setNick(const std::string& nick) 
{
    this->_nick = nick;
}

void Client::setUsername(const std::string& username) 
{
    this->_username = username;
}

void Client::setRealname(const std::string& realname) 
{
    this->_realname = realname;
}

const std::string& Client::getNick() const 
{
    return (this->_nick);
}

const std::string& Client::getUsername() const 
{
    return (this->_username);
}

const std::string& Client::getRealname() const {
    return (this->_realname);
}

void Client::sendReply(const std::string& reply) 
{
    std::string msg = reply + "\r\n";
    send(_sock_fd, msg.c_str(), msg.size(), 0);
}

std::ostream& operator<<( std::ostream& os, const Client& other ) {
    os << "=== New user infos ===" << std::endl;
    os << "Nick: " << other.getNick() << std::endl;
    os << "User: " << other.getUsername() << std::endl;
    os << "Realname: " << other.getRealname() << std::endl;
    os << "======================";
    return (os);
}

const std::string& Client::getBuffer() const 
{ 
    return _buffer; 
}

void Client::eraseBuffer(size_t len) 
{ 
    _buffer.erase(0, len); 
}

int Client::getSockFd() const 
{ 
    return _sock_fd; 
}