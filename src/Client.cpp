#include "Client.hpp"

Client::Client() {}

Client::Client( int sock_fd, sockaddr_in client, socklen_t client_size ) 
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

void Client::handleCAPLS()
{
    std::string reply = "CAP * LS :\r\n";
    send(_sock_fd, reply.c_str(), reply.size(), 0);
    _buffer.clear();
}

void Client::handleCAPEND()
{
    _buffer.clear();
}

void Client::handleUserInfos()
{
    std::string nick = _buffer.substr(_buffer.find("NICK ") + 5);
    nick = nick.substr(0, nick.find("\r\n"));
    this->setNick(nick);

    std::string user = _buffer.substr(_buffer.find("USER") + 5);
    user = user.substr(0, user.find(" "));
    this->setUsername(user);

    std::string realname = _buffer.substr(_buffer.find(":") + 1);
    realname = realname.substr(0, realname.find("\r\n"));
    this->setRealname(user);

    std::string welcome = ":" + std::string(this->_serv) + " 001 " + nick + " :Welcome\r\n";
    send(_sock_fd, welcome.c_str(), welcome.size(), 0);

    _buffer.clear();
    std::cout << *this << std::endl;
}


void Client::handleBufferData()
{
    if (_buffer.find("CAP LS") != std::string::npos)
        this->handleCAPLS();
    else if (_buffer.find("CAP END") != std::string::npos)
        this->handleCAPEND();
    else if (_buffer.find("NICK ") != std::string::npos 
          && _buffer.find("USER ") != std::string::npos)
        this->handleUserInfos();
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

void Client::sendReply(const std::string& reply) { (void)reply; return ; }

std::ostream& operator<<( std::ostream& os, const Client& other ) {
    os << "=== New user infos ===" << std::endl;
    os << "Nick: " << other.getNick() << std::endl;
    os << "User: " << other.getUsername() << std::endl;
    os << "Realname: " << other.getRealname() << std::endl;
    os << "======================";
    return (os);
}
