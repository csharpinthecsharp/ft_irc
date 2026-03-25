#pragma once

#include "../Message.hpp"
#include "../Channel/Channel.hpp"
#include <map>

class Client;

void handlePass(const Message& msg, Client& client, const  std::string& serverPassword);
void handleNick(const Message& msg, Client& client, std::map<int, Client>& clients);
void handleUser(const Message& msg, Client& client);
void handleCap(const Message& msg, Client& client);
void handleJoin(const Message& msg, Client& client, std::map<std::string, Channel>& channels);