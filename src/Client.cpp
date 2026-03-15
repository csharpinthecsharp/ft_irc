#include "Client.hpp"

Client::Client( int sock_fd, sockaddr_in client, socklen_t client_size ) 
: _sock_fd(0),
_host(),
_serv(),
_clientSize(0),
_client()
{
    this->_sock_fd = sock_fd;
    this->_client = client;
    this->_clientSize = client_size;
    this->fillNameInfo();
}

Client::~Client() {}


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
