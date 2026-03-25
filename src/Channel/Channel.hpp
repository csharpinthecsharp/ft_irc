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
        std::map<int, Client*> _members;
        std::vector<int> _operators;

    public:
        Channel();
        Channel(const std::string& name);
        ~Channel();

        const std::string& getName() const;
        const std::string& getTopic() const;
        void setTopic(const std::string& topic);

        void addMember(Client* client);
        void removeMember(Client* client);
        bool isMember(Client* client) const;

        void addOperator(int fd); // operator = modo (kick topic mode invite)
        bool isOperator(int fd) const;

        void broadcast(const std::string& msg, int exclude_fd = -1);

        const std::map<int, Client*>& getMembers() const;
};