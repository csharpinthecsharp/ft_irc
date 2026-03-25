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

void handleJoin(const Message& msg, Client& client, std::map<std::string, Channel>& channels)
{
    if (!client.isRegistered())
    {
        client.sendReply(":ircserv 451 * :You have not registered");
        return ;
    }
    if (msg.getParams().empty())
    {
        client.sendReply(":ircserv 461 * :Not enough parameters");
        return;
    }
    std::string channelName = msg.getParams()[0];
    if (channelName[0] != '#')
    {
        client.sendReply(":ircserv 403 " + client.getNick() + " " + channelName + " :No such channel");
        return ;
    }
    if (channels.find(channelName) == channels.end()) // creation si le channel existe pas
        channels[channelName] = Channel(channelName);

    Channel& channel = channels[channelName];
    if (channel.isMember(&client))
        return;
    channel.addMember(&client);

    if (channel.getMembers().size() == 1) // premier membre est operator
        channel.addOperator(client.getSockFd());
    std::string joinMsg = ":" + client.getNick() + "!~" + client.getUsername() + "@localhost JOIN " + channelName;
    channel.broadcast(joinMsg); // notifier le channel des nouveaux

    if (!channel.getTopic().empty())
        client.sendReply(":ircserv 332 " + client.getNick() + " " + channelName + " :" + channel.getTopic());
    else    
        client.sendReply(":ircserv 331 " + client.getNick() + " " + channelName + " :No topic is set");
    std::string namesList = ":ircserv 353 " + client.getNick() + " = " + channelName + " :";
    const std::map<int, Client*>& members = channel.getMembers();
    std::map<int, Client*>::const_iterator it;
    for (it = members.begin(); it != members.end(); it++)
    {
        if (channel.isOperator(it->first))
            namesList += "@";
        namesList += it->second->getNick() + " ";
    }
    client.sendReply(namesList);
    client.sendReply(":ircserv 366 " + client.getNick() + " " + channelName + " :End of /NAMES list");
}   