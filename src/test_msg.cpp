#include "Message.hpp"
#include <iostream>

void test(std::string raw) {
    Message msg(raw);
    std::cout << "Command: [" << msg.getCommand() << "]" << std::endl;
    std::cout << "Params size: " << msg.getParams().size() << std::endl;
    for (size_t i = 0; i < msg.getParams().size(); ++i) {
        std::cout << "  param[" << i << "]: " << msg.getParams()[i] << std::endl;
    }
    std::cout << "Message: [" << msg.getMessage() << "]" << std::endl;
}

int main() {
    test("NICK flo\r\n");
    test("PRIVMSG #42 :Salut tout le monde");
    test(":pseudo!user@host JOIN #canal");
    test("KICK #canal flo :raison du kick");
    
    return 0;
}