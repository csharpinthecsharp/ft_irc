#include "Channel.hpp"
#include "../Client.hpp"

Channel::Channel() {}

Channel::Channel(const std::string& name) : _name(name), _topic("") {}

Channel::~Channel() {}

const std::string& Channel::getName() const { return _name; }
const std::string& Channel::getTopic() const { return _topic; }
void Channel::setTopic(const std::string& topic) { _topic = topic; }

void Channel::addMember(Client* client)
{
    _members[client->getSockFd()] = client;
}

void Channel::removeMember(Client* client)
{
    _members.erase(client->getSockFd());
}

bool Channel::isMember(Client* client) const
{
    return _members.find(client->getSockFd()) != _members.end();
}

void Channel::addOperator(int fd)
{
    _operators.push_back(fd);
}

bool Channel::isOperator(int fd) const
{
    for (size_t i = 0; i < _operators.size(); i++)
    {
        if (_operators[i] == fd)
            return true;
    }
    return false;
}

void Channel::broadcast(const std::string& msg, int exclude_fd)
{
    std::map<int, Client*>::iterator it;
    for (it = _members.begin(); it != _members.end(); it++)
    {
        if (it->first != exclude_fd)
            it->second->sendReply(msg);
    }
}

const std::map<int, Client*>& Channel::getMembers() const
{
    return _members;
}