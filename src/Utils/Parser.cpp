#include "Parser.hpp"
#include "Exceptions.hpp"

const std::string server_password = "1234";

Parser::Parser() {}
Parser::Parser( std::string port, std::string psswrd ) :
_port(0), _psswrd(psswrd)
{
    size_t port_len = port.length();
    /* PORT EST IL DE LA BONNE TAILLE ? */
    if (port_len > PORT_MAX_SIZE || port_len < PORT_MIN_SIZE) 
        throw WrongPortSizeException();
    /* PORT EST-IL SEULEMENT DIGIT ? */
    for (size_t i(0); i < port_len; ++i) 
    {
        if (!isdigit(port[i]))
            throw WrongDigitPortException();
    }
    /* PASSWORD CHECK */
    if (psswrd != server_password)
        throw WrongPasswordException();
    /* STD::STRING -> UNSIGNED INT */
    std::stringstream ss(port);
    ss >> this->_port;
}
Parser::~Parser() 
{

}

unsigned int Parser::getPort() const 
{
    return this->_port;
}
std::string Parser::getPsswrd() const 
{
    return this->_psswrd;
}

std::ostream &operator<<( std::ostream& os, const Parser& other ) 
{
    os << "Port: " << other.getPort() << std::endl;
    os << "Psswrd: " << other.getPsswrd();
    return os;
}