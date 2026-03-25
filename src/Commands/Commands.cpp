#include "Commands.hpp"
#include "../Client.hpp"

void handlePass(const Message& msg, Client& client, const std::string& serverPassword)
{
    if (client.isAuthenticated()) 
    {
        client.sendReply(":ircserv 462 * :You are already authenticated");
        return;
    }

    if (msg.getParams().empty()) 
    {
        client.sendReply(":ircserv 461 * :Not enough parameters");
        return;
    }

    if (msg.getParams()[0] != serverPassword) 
    {
        client.sendReply(":ircserv 464 * :Password incorrect");
        client.shutdown();
        return;
    }
    client.setAuthenticated(true);
    client.tryToRegister();
}

void handleNick(const Message& msg, Client& client, std::map<int, Client>& clients)
{
    if (msg.getParams().empty()) {
        client.sendReply(":ircserv 431 * :No nickname given");
        return;
    }
    std::string newNick = msg.getParams()[0];
    std::map<int, Client>::iterator it;
    for (it = clients.begin(); it != clients.end(); it++)
    {
        if (it->second.getNick() == newNick) {
            client.sendReply(":ircserv 433 * " + newNick + " :Nickname already in use");
            return;
        }
    }
    client.setNick(newNick);
    client.tryToRegister();
}

void handleUser(const Message& msg, Client& client) 
{
    if (msg.getParams().size() < 3 || msg.getMessage().empty()) 
    {
        client.sendReply(":ircserv 461 * :User not enough parameters");
        return ;
    }
    client.setUsername(msg.getParams()[0]);
    client.setRealname(msg.getMessage());

    client.tryToRegister();
}

void handleCap(const Message& msg, Client& client)
{
    if (msg.getParams().empty()) 
        return;
    if (msg.getParams()[0] == "LS")
        client.sendReply("CAP * LS :");
    else if (msg.getParams()[0] == "END")
        client.eraseBuffer(client.getBuffer().size());
}