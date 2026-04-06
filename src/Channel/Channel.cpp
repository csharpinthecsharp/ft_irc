#include "Channel.hpp"
#include "../Client.hpp"

Channel::Channel() {}

Channel::Channel(const std::string& name) : _name(name), _topic("") {}

Channel::~Channel() {}

const std::string& Channel::getName() const { return _name; }
const std::string& Channel::getTopic() const { return _topic; }
void Channel::setTopic(const std::string& topic) { _topic = topic; }

void Channel::addMember(int fd)
{
    if (!isMember(fd))
        _members.push_back(fd);
}

void Channel::removeMember(int fd)
{
    for (size_t i = 0; i < _members.size(); i++)
    {
        if (_members[i] == fd)
        {
            _members.erase(_members.begin() + i);
            return;
        }
    }
}

bool Channel::isMember(int fd) const
{
    for (size_t i = 0; i < _members.size(); i++)
    {
        if (_members[i] == fd)
            return true;  
    }
    return false;
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

void Channel::broadcast(const std::string& msg, std::map<int, Client>& clients, int exclude_fd)
{
    for (size_t i = 0; i < _members.size(); i++)
    {
        if (_members[i] == exclude_fd)
            continue;
        std::map<int, Client>::iterator it = clients.find(_members[i]);
        if (it != clients.end())
            it->second.sendReply(msg);
    }
}

const std::vector<int>& Channel::getMembers() const
{
    return _members;
}

void Channel::addInvited(int fd)
{
    if (!isInvited(fd))
        _invited.push_back(fd);
}

bool Channel::isInvited(int fd) const
{
    for (size_t i = 0; i < _invited.size(); i++)
    {
        if (_invited[i] == fd)
            return true;
    }
    return false;
}

void Channel::removeInvited(int fd)
{
    for (size_t i = 0; i < _invited.size(); i++)
    {
        if (_invited[i] == fd)
        {
            _invited.erase(_invited.begin() + i);
            return;
        }
    }
}

void Channel::promoteNextOperator(std::map<int, Client>& clients)
{
    if (_members.empty())
        return;
    if (!_operators.empty())
        return;

    int newOp = _members[0];
    _operators.push_back(newOp);

    std::map<int, Client>::iterator it = clients.find(newOp);
    if (it != clients.end())
        broadcast(":ircserv MODE " + _name + " +o " + it->second.getNick(), clients);
}