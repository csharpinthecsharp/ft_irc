#pragma once

#include "../Message.hpp"
#include "../Client.hpp"

void handlePass(const Message& msg, Client& client, const  std::string& serverPassword);
void handleNick(const Message& msg, Client& client);
void handleUser(const Message& msg, Client& client);

