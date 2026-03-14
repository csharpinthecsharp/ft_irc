#include "Client.hpp"

Client::Client( const Server& server ) 
: _loc_listen_fd(0),
_host(),
_serv(),
_clientSize(0),
_client()
{
    this->_loc_listen_fd = server.getFd();
    this->acceptCall();
    this->fillNameInfo();
}

Client::~Client() {}

void Client::acceptCall() 
{
    _clientSize = sizeof(_client);

    int clientSocket = accept(this->_loc_listen_fd,
                            (sockaddr*)&_client,
                            &_clientSize);
    if (clientSocket == FAIL)
        throw ClientSocketCreationFailedException();
    close(this->_loc_listen_fd);
    return ;
}

void Client::fillNameInfo() 
{
    int res = getnameinfo((sockaddr*)&_client,
                _clientSize,
                _host,
                NI_MAXHOST,
                _serv, NI_MAXSERV, 0);

    if (res != 0)
        throw GettingNameInfoFailedException();    
    else
        std::cout << _host << " connect on " << _serv << std::endl;
    return ;
        
}
