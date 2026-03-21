#include "Server.hpp"

Server::Server( const Parser& parser ) 
: _listen_fd(0),
_clients()
{
    this->_loc_port = parser.getPort();
    this->_listen_fd = this->createSocket();
    this->markSocket();
}
Server::~Server() 
{
    close(this->_listen_fd);
}

int Server::createSocket() const 
{
    // AF_INET = IPV4 (domain);
    // SOCK_STREAM = Connection-oriented communication (type);
    // IPPROTO_TCP = PROTOCOL (TCP);
    int temp = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (temp == FAIL)
        throw ServerSocketCreationFailedException();
    return temp;
}

void Server::markSocket() const
{
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(this->_loc_port); // htons > flip bits 
    hint.sin_addr.s_addr = INADDR_ANY;

    // Bind PORT/IP
    if (bind(this->_listen_fd, (sockaddr*)&hint, sizeof(hint)) == FAIL)
        throw BindFailedExpection();

    // Mark for listening
    if (listen(this->_listen_fd, SOMAXCONN) == FAIL)
        throw ListenFailedExpection();
     return ;
}

void Server::open() {
    std::vector<struct pollfd> all_poll;

    struct pollfd l_poll;
    l_poll.fd = this->_listen_fd;
    l_poll.events = POLLIN;
    l_poll.revents = 0;
    all_poll.push_back(l_poll);

    while (1) 
    {
        // Va fill revents
        if (poll(all_poll.data(), all_poll.size(), -1) == FAIL)
            throw MainPolledFailedException();

        for (size_t i(0); i < all_poll.size(); ++i)
        {

            // revents doit etre check maintenant qu'il a été fill par poll(),
            // si il est vide rien ne s'est produit.
            if (all_poll[i].revents & POLLIN)
                continue;
            
            // cas 1 -> Nouveau client veut se connecter
            // cas 2 -> Un client existant envoie de la data    
            if (all_poll[i].fd == this->_listen_fd)
            {
                sockaddr_in client;
                socklen_t clientSize = sizeof(client);

                int client_fd = accept(this->_listen_fd,
                                        (sockaddr*)&client,
                                        &clientSize);
                if (client_fd == FAIL)
                    throw ClientSocketCreationFailedException();
                
                _clients.insert({client_fd, Client(client_fd, client, clientSize)});

                struct pollfd c_poll;
                c_poll.fd = this->_listen_fd;
                c_poll.events = POLLIN;
                c_poll.revents = 0;
                all_poll.push_back(c_poll);
            }
            else
            {
            // UN MESSAGE
                char buffer[IRCMAXBUFFSIZE_MSG];
                ssize_t data = recv(all_poll[i].fd, &buffer, sizeof(buffer) - 1, 0);

                if (data > 0)
                {
                    buffer[data] = '\0';
                    // Append le client buffer + et proccess.
                }
                else
                {
                    close(all_poll[i].fd);
                    _clients.erase(all_poll[i].fd);
                    all_poll.erase(all_poll.begin() + i);
                    i--;
                }
            }
        }
        
        //_clients[clientSocket].dzdzdzdzd;
        // Maintenant je dois répondre à (17:02 Waiting for CAP LS response...)
    }
    //close(this->_listen_fd);
}


