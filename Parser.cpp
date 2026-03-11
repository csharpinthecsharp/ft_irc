#include "Parser.hpp"

Parser::Parser() {}
Parser::Parser( std::string port, std::string psswrd ) :
_port(0), _psswrd(psswrd)
{
    size_t port_len = port.length();
    if (port_len > PORT_MAX_SIZE || port_len < PORT_MIN_SIZE) 
        throw WrongPortSizeException();
    for (size_t i(0); i < port_len; ++i) {
        if (!isdigit(port[i]))
            throw WrongDigitPortException();
    }
    std::stringstream ss(port);
    ss >> this->_port;
}
Parser::~Parser() {

}

unsigned int Parser::getPort() const {
    return this->_port;
}
std::string Parser::getPsswrd() const {
    return this->_psswrd;
}

std::ostream &operator<<( std::ostream& os, const Parser& other ) {
    os << "Port: " << other.getPort() << std::endl;
    os << "Psswrd: " << other.getPsswrd();
    return os;
}