#include "Channel.hpp"
#include "../Client.hpp"

Channel::Channel() {}

Channel::Channel(const std::string& name) : _name(name), _topic(""), _locked(false) {}

Channel::~Channel() {}

const std::string& Channel::getName() const { return _name; }
const std::string& Channel::getTopic() const { return _topic; }
void Channel::setTopic(const std::string& topic) { _topic = topic; }

void Channel::addMember(int fd)
{
    // ON DOIT AJOUTé LES ERREURS POUR USER LIMIT && LOCKED.
    // + PASSWORD HANDLING SUR LE CHANNEL
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

void Channel::addLock()
{
    if (_locked)
        return;
    _locked = true;
}

void Channel::removeLock()
{
    if (!_locked)
        return;
    _locked = false;
}

bool Channel::isLocked() const
{
    return (_locked);
}

void Channel::addTopicLock()
{
    if (_topic_locked_for_operator)
        return;
    _topic_locked_for_operator = true;
}

void Channel::removeTopicLock()
{
    if (!_topic_locked_for_operator)
        return;
    _topic_locked_for_operator = false;
}

bool Channel::isTopicLocked() const
{
    return (_topic_locked_for_operator);
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

void Channel::addUserLimit( size_t n )
{
    this->_user_limit = n;
    return;
}

void Channel::removeUserLimit()
{
    this->_user_limit = 0;
    return;
}

bool Channel::isUserLimit() const
{
    // 0 = no limit
    return (this->_user_limit != 0 && this->_members.size() >= this->_user_limit);
}

void Channel::addPassword( const std::string& pass )
{
    this->_password = pass;
    return;
}

void Channel::removePassword()
{
    this->_password.clear();
    return;
}

const std::string& Channel::getPassword() const
{
    return (_password);
}


bool Channel::isPassword() const
{
    if (this->_password.empty())
        return false;
    return true;
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