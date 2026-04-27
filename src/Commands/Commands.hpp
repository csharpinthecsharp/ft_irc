#pragma once

#include "../Message.hpp"
#include "../Channel/Channel.hpp"
#include <map>
#include <set>

class Client;

void handlePass(const Message& msg, Client& client, const std::string& serverPassword);
void handleNick(const Message& msg, Client& client, std::map<int, Client>& clients, std::map<std::string, Channel>& channels);
void handleUser(const Message& msg, Client& client);
void handleCap(const Message& msg, Client& client);
void handleJoin(const Message& msg, Client& client, std::map<int, Client>& clients, std::map<std::string, Channel>& channels);
void handleTopic(const Message& msg, Client& client, std::map<int, Client>& clients, std::map<std::string, Channel>& channels);
void handlePrivmsg(const Message& msg, Client& client, std::map<int, Client>& clients, std::map<std::string, Channel>& channels);
void handlePart(const Message& msg, Client& client, std::map<int, Client>& clients, std::map<std::string, Channel>& channels);
void handleKick(const Message& msg, Client& client, std::map<int, Client>& clients, std::map<std::string, Channel>& channels);
void handleInvite(const Message& msg, Client& client, std::map<int, Client>& clients, std::map<std::string, Channel>& channels);
void handleQuit(const Message& msg, Client& client, std::map<int, Client>& clients, std::map<std::string, Channel>& channels);
void handleMode(const Message& msg, Client& client, std::map<int, Client>& clients, std::map<std::string, Channel>& channels);