#include "Utils/Parser.hpp"

int main( int ac, char **av)
{
    if (ac != 3)
    {
        std::cout << "ERROR: Please use > ./ircserv <port> <password>" << std::endl;
        return (1);
    }
    try 
    {
        Parser pr(av[1], av[2]);
        std::cout << pr << std::endl; 
    }
    catch (std::exception & e) 
    {
        std::cout << e.what() << std::endl;
    }
    return (0);
}