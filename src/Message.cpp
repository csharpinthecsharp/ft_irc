#include "Message.hpp"

Message::Message(std::string input) : _raw(input) {
    if (input.size() > 512) {
        input = input.substr(0, 512);
    }
    size_t last = input.find_last_not_of("\r\n");
    if (last != std::string::npos) {
        input = input.substr(0, last + 1);
    }

    size_t pos_message = input.find(" :");
    if (pos_message != std::string::npos) {
        _message = input.substr(pos_message + 2);
        input = input.substr(0, pos_message);
    }

    std::stringstream ss(input);
    std::string word;

    if (ss >> word) {
        if (word[0] == ':') {
            _prefix = word.substr(1);
            if (!(ss >> word))
                return;
            _prefix = word.substr(1);
            if (!(ss >> word))  return;
        }
        _command = word;
        for (size_t i = 0; i < _command.size(); ++i) {
            _command[i] = std::toupper(_command[i]);
        }
    }
    while (ss >> word) {
        _params.push_back(word);
    }
}

Message::~Message() {}

const std::string& Message::getCommand() const {
    return this->_command;
}

const std::vector<std::string>& Message::getParams() const {
    return this->_params;
}

const std::string& Message::getMessage() const {
    return this->_message;
}