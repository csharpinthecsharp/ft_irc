#pragma once

#include "../Message.hpp"

class Client;

void handlePass(const Message& msg, Client& client, const  std::string& serverPassword);
void handleNick(const Message& msg, Client& client);
void handleUser(const Message& msg, Client& client);
void handleCap(const Message& msg, Client& client);
