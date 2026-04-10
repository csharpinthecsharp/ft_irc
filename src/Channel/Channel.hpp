#pragma once

#include <string>
#include <map>
#include <vector>
#include <iostream>

class Client;

class Channel
{
    private:
        std::string _name;
        std::string _topic;
        std::vector<int> _members;
        std::vector<int> _operators;
        std::vector<int> _invited;
        std::string _password;
        size_t _user_limit;
        bool _locked;
        bool _topic_locked_for_operator;
    public:
        Channel();
        Channel(const std::string& name);
        ~Channel();

        const std::string& getName() const;
        const std::string& getTopic() const;
        void setTopic(const std::string& topic);

        void addMember(int fd);
        void removeMember(int fd);
        bool isMember(int fd) const;

        void addInvited(int fd);
        bool isInvited(int fd) const;
        void removeInvited(int fd);

        void addUserLimit( size_t n );
        void removeUserLimit();
        bool isUserLimit() const;

        void addPassword( const std::string& pass );
        void removePassword();
        bool isPassword() const;

        void addLock();
        void removeLock();
        bool isLocked() const;

        void addTopicLock();
        void removeTopicLock();
        bool isTopicLocked() const;

        void addOperator(int fd); // operator = modo (kick topic mode invite)
        bool isOperator(int fd) const;
        void promoteNextOperator(std::map<int, Client>& clients);

        void broadcast(const std::string& msg, std::map<int, Client>& clients, int exclude_fd = -1);
        const std::vector<int>& getMembers() const;
};