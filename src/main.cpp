#include "Utils/Parser.hpp"
#include "Server.hpp"
#include "Client.hpp"

int main( int ac, char **av)
{
    if (ac != 3)
    {
        std::cout << "ERROR: Please use > ./ircserv <port> <password>" << std::endl;
        return (1);
    }
    try 
    {
        Parser parser(av[1], av[2]);
        std::cout << parser << std::endl;
        Server server(parser);
        Client client(server);
    }
    catch (std::exception & e) 
    {
        std::cout << e.what() << std::endl;
    }
    return (0);
}