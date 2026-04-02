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
}

void handleJoin(const Message& msg, Client& client, std::map<int, Client>& clients, std::map<std::string, Channel>& channels)
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

    if (channel.isMember(client.getSockFd()))
        return;
    channel.addMember(client.getSockFd());

    if (channel.getMembers().size() == 1) // premier membre est operator
        channel.addOperator(client.getSockFd());

    std::string joinMsg = ":" + client.getNick() + "!~" + client.getUsername() + "@localhost JOIN " + channelName;
    channel.broadcast(joinMsg, clients); // notifier le channel des nouveaux

    if (!channel.getTopic().empty())
        client.sendReply(":ircserv 332 " + client.getNick() + " " + channelName + " :" + channel.getTopic());
    else    
        client.sendReply(":ircserv 331 " + client.getNick() + " " + channelName + " :No topic is set");

    std::string namesList = ":ircserv 353 " + client.getNick() + " = " + channelName + " :";
    const std::vector<int>& members = channel.getMembers();
    for (size_t i = 0; i < members.size(); i++)
    {
        std::map<int, Client>::iterator it = clients.find(members[i]);
        if (it == clients.end()) continue;
            if (channel.isOperator(members[i]))
                namesList += "@";
        namesList += it->second.getNick() + " ";
    }
    client.sendReply(namesList);
    client.sendReply(":ircserv 366 " + client.getNick() + " " + channelName + " :End of /NAMES list");
}

void handlePart(const Message& msg, Client& client, std::map<int, Client>& clients, std::map<std::string, Channel>& channels)
{
    if (!client.isRegistered())
    {
        client.sendReply(":ircserv 451 * :You have not registered");
        return;
    }
    if (msg.getParams().empty())
    {
        client.sendReply(":ircserv 461 PART :Not enough parameters");
        return;
    }

    std::string channelName = msg.getParams()[0];
    if (channelName[0] != '#')
    {
        client.sendReply(":ircserv 403 " + client.getNick() + " " + channelName + " :No such channel");
        return;
    }

    std::map<std::string, Channel>::iterator it = channels.find(channelName);
    if (it == channels.end())
    {
        client.sendReply(":ircserv 403 " + client.getNick() + " " + channelName + " :No such channel");
        return;
    }
    Channel& channel = it->second;

    if (!channel.isMember(client.getSockFd()))
    {
        client.sendReply(":ircserv 442 " + client.getNick() + " " + channelName + " :You're not on that channel");
        return;
    }

    std::string partMsg = ":" + client.getNick() + " PART " + channelName + " :"
        + (msg.getMessage().empty() ? "Good bye!" : msg.getMessage());
    channel.broadcast(partMsg, clients);
    channel.removeMember(client.getSockFd());
}

void handleTopic(const Message& msg, Client& client, std::map<int, Client>& clients, std::map<std::string, Channel>& channels)
{
    if (!client.isRegistered())
    {
        client.sendReply(":ircserv 451 * :You have not registered");
        return;
    }
    if (msg.getParams().empty())
    {
        client.sendReply(":ircserv 461 TOPIC :Not enough parameters");
        return;
    }

    std::string channelName = msg.getParams()[0];
    if (channelName[0] != '#')
    {
        client.sendReply(":ircserv 403 " + client.getNick() + " " + channelName + " :No such channel");
        return;
    }

    std::map<std::string, Channel>::iterator it = channels.find(channelName);
    if (it == channels.end())
    {
        client.sendReply(":ircserv 403 " + client.getNick() + " " + channelName + " :No such channel");
        return;
    }

    Channel& channel = it->second;
    if (!channel.isMember(client.getSockFd()))
    {
        client.sendReply(":ircserv 442 " + channelName + " :You're not on that channel");
        return;
    }

    if (msg.getMessage().empty())
    {
        if (channel.getTopic().empty())
            client.sendReply(":ircserv 331 " + client.getNick() + " " + channelName + " :No topic is set");
        else
            client.sendReply(":ircserv 332 " + client.getNick() + " " + channelName + " :" + channel.getTopic());
        return;
    }

    if (!channel.isOperator(client.getSockFd()))
    {
        client.sendReply(":ircserv 482 " + channelName + " :You're not channel operator");
        return;
    }
    channel.setTopic(msg.getMessage());
    channel.broadcast(":" + client.getNick() + " TOPIC " + channelName + " :" + msg.getMessage(), clients);
}

void handlePrivmsg(const Message& msg, Client& client, std::map<int, Client>& clients, std::map<std::string, Channel>& channels)
{
    if (!client.isRegistered())
    {
        client.sendReply(":ircserv 451 * :You have not registered");
        return;
    }
    if (msg.getParams().empty())
    {
        client.sendReply(":ircserv 411 " + client.getNick() + " :No recipient given");
        return;
    }
    if (msg.getMessage().empty())
    {
        client.sendReply(":ircserv 412 " + client.getNick() + " :No text to send");
        return;
    }

    std::string target = msg.getParams()[0];
    std::string fullMsg = ":" + client.getNick() + "!~" + client.getUsername()
                        + "@localhost PRIVMSG " + target + " :" + msg.getMessage();

    if (target[0] == '#')
    {
        if (channels.find(target) == channels.end())
        {
            client.sendReply(":ircserv 403 " + client.getNick() + " " + target + " :No such channel");
            return;
        }
        Channel& channel = channels[target];
        if (!channel.isMember(client.getSockFd()))
        {
            client.sendReply(":ircserv 404 " + client.getNick() + " " + target + " :Cannot send to channel");
            return;
        }
        channel.broadcast(fullMsg, clients, client.getSockFd());
    }
    else
    {
        std::map<int, Client>::iterator it;
        for (it = clients.begin(); it != clients.end(); it++)
        {
            if (it->second.getNick() == target)
            {
                it->second.sendReply(fullMsg);
                return;
            }
        }
        client.sendReply(":ircserv 401 " + client.getNick() + " " + target + " :No such nick");
    }
}

void handleKick(const Message& msg, Client& client, std::map<int, Client>& clients, std::map<std::string, Channel>& channels)
{
    if (!client.isRegistered())
    {
        client.sendReply(":ircserv 451 * :You have not registered");
        return;
    }
    if (msg.getParams().size() < 2)
    {
        client.sendReply(":ircserv 461 KICK :Not enough parameters");
        return;
    }

    std::string channelName = msg.getParams()[0];
    std::string targetNick = msg.getParams()[1];
    std::string reason;
    if (!msg.getMessage().empty())
        reason = msg.getMessage();
    else if (msg.getParams().size() > 2)
        reason = msg.getParams()[2];
    else
        reason = "No reason";

    std::map<std::string, Channel>::iterator it = channels.find(channelName);
    if (it == channels.end())
    {
        client.sendReply(":ircserv 403 " + client.getNick() + " " + channelName + " :No such channel");
        return;
    }
    Channel& channel = it->second;
    
    if (!channel.isMember(client.getSockFd()))
    {
        client.sendReply(":ircserv 442 " + client.getNick() + " " + channelName + " :You're not on that channel");
        return;
    }
    if (!channel.isOperator(client.getSockFd()))
    {
        client.sendReply(":ircserv 482 " + client.getNick() + " " + channelName + " :You're not channel operator");
        return;
    }
    std::map<int, Client>::iterator target;
    for (target = clients.begin(); target != clients.end(); target++)
    {
        if (target->second.getNick() == targetNick)
            break;
    }
    if (target == clients.end())
    {
        client.sendReply(":ircserv 401 " + client.getNick() + " " + targetNick + " :No such nick");
        return;
    }
    if (!channel.isMember(target->second.getSockFd()))
    {
        client.sendReply(":ircserv 441 " + client.getNick() + " " + targetNick + " :They aren't on that channel");
        return;
    }
    std::string kickMsg = ":" + client.getNick() + "!~" + client.getUsername()
        + "@localhost KICK " + channelName + " " + targetNick + " :" + reason;
    channel.broadcast(kickMsg, clients);
    channel.removeMember(target->second.getSockFd());
}