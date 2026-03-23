#include "Commands.hpp"
#include "../Client.hpp"

void handlePass(const Message& msg, Client& client, const std::string& serverPassword)
{
    if (client.isAuthenticated()) 
    {
        client.sendReply("462 :You are already registered");
        return ;
    }
    if (msg.getParams().empty()) 
    {
        client.sendReply("461 :Not enough parameters");
        return ;
    }
    if (msg.getParams()[0] != serverPassword) 
    {
        client.sendReply("464 :Incorrect password try again");
        return ;
    }
    client.setAuthenticated(true);
}

void handleNick(const Message& msg, Client& client)
{
    if (!client.isAuthenticated()) 
    {
        client.sendReply("451 :You have not registered");
        return ;
    }
    if (msg.getParams().empty()) 
    {
        client.sendReply("431 :No nickname given");
        return ;
    }
    client.setNick(msg.getParams()[0]);
    // todo : verifier si le nickname n'est pas deja pris dans le serv
}

void handleUser(const Message& msg, Client& client) 
{
    if (!client.isAuthenticated()) 
    {
        client.sendReply("451 :You have not registered");
        return ;
    }
    if (client.isRegistered()) 
    {
        client.sendReply("462 :You are already registered");
        return ;
    }
    if (msg.getParams().size() < 3 || msg.getMessage().empty()) 
    {
        client.sendReply("461 : User not enough parameters");
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