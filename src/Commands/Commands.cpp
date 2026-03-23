#include "Commands.hpp"
#include "../Client.hpp"

void handlePass(const Message& msg, Client& client, const std::string& serverPassword)
{
    if (client.isAuthenticated()) 
    {
        client.sendReply(":ircserv 462 * :You are already registered");
        return ;
    }
    if (msg.getParams().empty()) 
    {
        client.sendReply(":ircserv 461 * :Not enough parameters");
        return ;
    }
    if (msg.getParams()[0] != serverPassword) 
    {
        client.sendReply(":ircserv 464 * :Incorrect password try again");
        return ;
    }
    client.setAuthenticated(true);
}

void handleNick(const Message& msg, Client& client, std::map<int, Client>& clients)
{
    if (!client.isAuthenticated()) {
        client.sendReply(":ircserv 451 * :You have not registered");
        return;
    }
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
}

void handleUser(const Message& msg, Client& client) 
{
    if (!client.isAuthenticated()) 
    {
        client.sendReply(":ircserv 451 * :You have not registered");
        return ;
    }
    if (client.isRegistered()) 
    {
        client.sendReply(":ircserv 462 * :You are already registered");
        return ;
    }
    if (msg.getParams().size() < 3 || msg.getMessage().empty()) 
    {
        client.sendReply(":ircserv 461 * :User not enough parameters");
        return ;
    }
    client.setUsername(msg.getParams()[0]);
    client.setRealname(msg.getMessage());

    if (!client.getNick().empty()) 
    {
        client.setRegistered(true);
        client.sendReply(":ircserv 001 " + client.getNick() + " :Welcome to the server!");
    }
}

void handleCap(const Message& msg, Client& client)
{
    if (msg.getParams().empty()) 
        return;
    if (msg.getParams()[0] == "LS")
        client.sendReply("CAP * LS :");
}