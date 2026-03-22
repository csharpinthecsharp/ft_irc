#include "Parser.hpp"

const std::string server_password = "1234";

Parser::Parser() {}
Parser::Parser( std::string port, std::string psswrd ) :
_port(0), _psswrd("")
{
    checkArguments(port, psswrd);
    _psswrd = psswrd;
    std::stringstream ss(port);
    ss >> this->_port;
}
Parser::~Parser() {}

void Parser::checkArguments( const std::string& port, const std::string& psswrd ) const {
    size_t port_len = port.length();
    if (port_len > PORT_MAX_SIZE || port_len < PORT_MIN_SIZE) 
        throw WrongPortSizeException();
    for (size_t i(0); i < port_len; ++i) 
    {
        if (!isdigit(port[i]))
            throw WrongDigitPortException();
    }
    if (psswrd != server_password)
        throw WrongPasswordException();
}

unsigned int Parser::getPort() const 
{
    return (this->_port);
}
std::string Parser::getPsswrd() const
{
    return (this->_psswrd);
}

std::ostream &operator<<( std::ostream& os, const Parser& other ) 
{
    os << "Port: " << other.getPort() << std::endl;
    os << "Psswrd: " << other.getPsswrd();
    return (os);
}