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
    if (client.isRegistered())
    {
        std::string oldNick = client.getNick();
        client.setNick(newNick);
        client.sendReply(":" + oldNick + "!" + client.getUsername() + "@" + client.getHost() + " NICK :" + newNick);
    }
    else
    {
        client.setNick(newNick);
    }
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

    if (channel.isLocked() && !channel.isInvited(client.getSockFd())) 
    {
        client.sendReply(":ircserv 473 " + client.getNick() + " " + channelName + " :Cannot join channel (+i)");
        return;
    }

    if (channel.isUserLimit()) 
    {
        client.sendReply(":ircserv 473 " + client.getNick() + " " + channelName + " :Cannot join channel (+l)");
        return;
    }

    if (channel.isPassword())
    {
        std::string key = msg.getParams()[1];
        if (key != channel.getPassword())
        {
            client.sendReply(":ircserv 475 " + channelName + " :Cannot join channel (+k)");
            return;
        }
    }

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

    // for mode t
    if (channel.isTopicLocked() && !channel.isOperator(client.getSockFd()))
    {
            client.sendReply(":ircserv 482 " + channelName + " :You're not channel operator");
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
    if (channel.isTopicLocked())
    {
        if (!channel.isOperator(client.getSockFd()))
        {
            client.sendReply(":ircserv 482 " + channelName + " :You're not channel operator");
            return;
        }
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


void handleInvite(const Message& msg, Client& client, std::map<int, Client>& clients, std::map<std::string, Channel>& channels)
{
    // todo : gerer +i quand il sera implemente
    if (!client.isRegistered())
    {
        client.sendReply(":ircserv 451 * :You have not registered");
        return;
    }
    if (msg.getParams().size() < 2)
    {
        client.sendReply(":ircserv 461 INVITE :Not enough parameters");
        return;
    }

    std::string targetNick = msg.getParams()[0];
    std::string channelName = msg.getParams()[1];

    std::map<std::string, Channel>::iterator chanIt = channels.find(channelName);
    if (chanIt == channels.end())
    {
        client.sendReply(":ircserv 403 " + client.getNick() + " " + channelName + " :No such channel");
        return;                                                                                          
    }
    Channel& channel = chanIt->second;

    if (!channel.isMember(client.getSockFd()))
    {
        client.sendReply(":ircserv 442 " + client.getNick() + " " + channelName + " :You're not on that channel");
        return;
    }

    std::map<int, Client>::iterator targetIt;
    for (targetIt = clients.begin(); targetIt != clients.end(); targetIt++)
    {
        if (targetIt->second.getNick() == targetNick)
            break;
    }
    if (targetIt == clients.end())
    {
        client.sendReply(":ircserv 401 " + client.getNick() + " " + targetNick + " :No such nick");
        return;
    }
    Client& target = targetIt->second;
    if (channel.isMember(target.getSockFd()))
    {
        client.sendReply(":ircserv 443 " + client.getNick() + " " + targetNick + " " + channelName + " :is already on channel");
        return;
    }
    channel.addInvited(target.getSockFd());
    client.sendReply(":ircserv 341 " + client.getNick() + " " + targetNick + " " + channelName);
    target.sendReply(":" + client.getNick() + "!~" + client.getUsername() + "@localhost INVITE " + targetNick + " :" + channelName);
}

void handleQuit(const Message& msg, Client& client, std::map<int, Client>& clients, std::map<std::string, Channel>& channels)
{
    std::string quitMsg = ":" + client.getNick() + "!~" + client.getUsername()
        + "@localhost QUIT :" + (msg.getMessage().empty() ? "Quit" : msg.getMessage());

    std::map<std::string, Channel>::iterator it;
    for (it = channels.begin(); it != channels.end(); it++)
    {
        Channel& chan = it->second;
        if (chan.isMember(client.getSockFd()))
        {
            chan.broadcast(quitMsg, clients, client.getSockFd());
            chan.removeMember(client.getSockFd());
            chan.promoteNextOperator(clients);
        }
    }
    client.sendReply(":ircserv ERROR :Goodbye");
}

void handleMode(const Message& msg, Client& client, std::map<int, Client>& clients, std::map<std::string, Channel>& channels) 
{
    ///mode #cool +o MEAT
    if (!client.isRegistered())
    {
        client.sendReply(":ircserv 451 * :You have not registered");
        return;
    }
    if (msg.getParams().size() < 2)
        return;

    std::map<std::string, Channel>::iterator it = channels.find(msg.getParams()[0]);
    if (it == channels.end())
        return;
    Channel& targetChannel = it->second;

    std::string mode = msg.getParams()[1];
    
    if (mode.size() != 2)
    {
        client.sendReply(":ircserv 472 " + client.getNick() + " " + mode + " :is unknown mode char");
        return;
    }
    
    bool set;
    if (mode[0] == '+')
        set = true;
    else if (mode[0] == '-')
        set = false;
    else
    {
        client.sendReply(":ircserv 472 " + client.getNick() + " " + mode + " :is unknown mode char");
        return;
    }

    char flag = mode[1];
    if (flag != 'i' && flag != 't' && flag != 'k' && flag != 'l' && flag != 'o')
    {
        client.sendReply(":ircserv 472 " + client.getNick() + " " + mode + " :is unknown mode char");
        return;
    }

    if ((flag == 'k' || flag == 'l' || flag == 'o') && set && msg.getParams().size() < 3)
    {
        client.sendReply(":ircserv 461 MODE :Not enough parameters");
        return;
    }

    std::string target = msg.getParams()[0];
    std::string lastParam;
    if (msg.getParams().size() > 2)
        lastParam = msg.getParams()[2];

    Client* user = NULL;

    if (!lastParam.empty())
    {
        for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it)
        {
            if (it->second.getNick() == lastParam)
            {
                user = &it->second;
                break;
            }
        }
        if (!user)
        {
            client.sendReply(":ircserv 401 " + client.getNick() + " " + lastParam + " :No such nick");
            return;
        }
    }
    
    switch (flag)
    {
        case 'i':
            set ? targetChannel.addLock() : targetChannel.removeLock();
            break;
        case 't':
            set ? targetChannel.addTopicLock() : targetChannel.removeTopicLock();
            break;
        case 'k':
            set ? targetChannel.addPassword(lastParam) : targetChannel.removePassword();
            break;
        case 'o':
            set ? targetChannel.addOperator(user->getSockFd()) : targetChannel.removeOperator(user->getSockFd());
            break;
        case 'l':
            set ? targetChannel.addUserLimit(std::atoi(lastParam.c_str()))
                : targetChannel.removeUserLimit();
            break;
    }
    return; 
}