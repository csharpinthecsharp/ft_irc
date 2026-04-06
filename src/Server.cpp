#include "Server.hpp"

Server::Server( const Parser& parser ) 
: _listen_fd(0),
_password(parser.getPsswrd()),
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
    int temp = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (temp == FAIL)
        throw ServerSocketCreationFailedException();
    return temp;
}

void Server::markSocket() const
{
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(this->_loc_port); 
    hint.sin_addr.s_addr = INADDR_ANY;

    if (bind(this->_listen_fd, (sockaddr*)&hint, sizeof(hint)) == FAIL)
        throw BindFailedExpection();

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
        if (poll(all_poll.data(), all_poll.size(), -1) == FAIL)
            throw MainPolledFailedException();

        for (size_t i(0); i < all_poll.size(); i++)
        {
            if (!(all_poll[i].revents & (POLLIN | POLLHUP | POLLERR)))
                continue;
            
            if (all_poll[i].fd == this->_listen_fd)
            {
                sockaddr_in client;
                socklen_t clientSize = sizeof(client);

                int client_fd = accept(this->_listen_fd,
                                        (sockaddr*)&client,
                                        &clientSize);
                if (client_fd == FAIL)
                    throw ClientSocketCreationFailedException();

                _clients[client_fd] = Client(client_fd, client, clientSize, _password);

                struct pollfd c_poll;
                c_poll.fd = client_fd;
                c_poll.events = POLLIN;
                c_poll.revents = 0;
                all_poll.push_back(c_poll);
            }
            else
            {
                char buffer[IRCMAXBUFFSIZE_MSG];
                ssize_t data = recv(all_poll[i].fd, &buffer, sizeof(buffer) - 1, 0);

                if (data > 0)
                {
                    buffer[data] = '\0';
                    std::cout << buffer << std::endl;
                    Client& c = _clients[all_poll[i].fd];
                    c.appendBuffer(buffer);

                    bool alive = true;
                    size_t pos;
                    while (alive && (pos = c.getBuffer().find("\r\n")) != std::string::npos)
                    {
                        std::string line = c.getBuffer().substr(0, pos);
                        c.eraseBuffer(pos + 2);
                        if (!line.empty())
                            alive = dispatch(Message(line), c);
                    }

                    if (!alive)
                    {
                        close(all_poll[i].fd);
                        _clients.erase(all_poll[i].fd);
                        all_poll.erase(all_poll.begin() + i);
                        i--;
                    }
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
    }
}

bool Server::dispatch(const Message& msg, Client& client)
{
    const std::string& cmd = msg.getCommand();
    if (cmd == "CAP")          
        handleCap(msg, client);
    else if (cmd == "PASS")    
        handlePass(msg, client, _password);
    else if (cmd == "NICK")
        handleNick(msg, client, _clients);
    else if (cmd == "USER")    
        handleUser(msg, client);
    else if (cmd == "PING")
    {
        std::string pong = "PONG :" + msg.getMessage() + "\r\n";
        send(client.getSockFd(), pong.c_str(), pong.size(), 0);
    }
    else if (cmd == "JOIN")
        handleJoin(msg, client, _clients, _channels);
    else if (cmd == "TOPIC")
        handleTopic(msg, client, _clients, _channels);
    else if (cmd == "PRIVMSG")
        handlePrivmsg(msg, client, _clients, _channels);
    else if (cmd == "PART")
        handlePart(msg, client, _clients, _channels);
    else if (cmd == "KICK")
        handleKick(msg, client, _clients, _channels);
    else if (cmd == "INVITE")
        handleInvite(msg, client, _clients, _channels);
    else if (cmd == "QUIT")
    {
        client.sendReply(":ircserv ERROR :Goodbye");    
        return false;
    }
    return true;
}
