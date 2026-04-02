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

        void addOperator(int fd); // operator = modo (kick topic mode invite)
        bool isOperator(int fd) const;

        void broadcast(const std::string& msg, std::map<int, Client>& clients, int exclude_fd = -1);
        const std::vector<int>& getMembers() const;
};