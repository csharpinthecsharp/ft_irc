#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>

class Message
{
    private:
        std::string _raw;
        std::string _command;
        std::vector<std::string> _params;
        std::string _message;
        std::string _prefix;
        
    public:
        Message(std::string input);
        ~Message();

        const std::string& getCommand() const;
        const std::vector<std::string>& getParams() const;
        const std::string& getMessage() const;
};
