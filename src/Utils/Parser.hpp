#ifndef PARSER_HPP
#define PARSER_HPP

#define PORT_MAX_SIZE 5
#define PORT_MIN_SIZE 1

#include <iostream>
#include <sstream>
#include <exception>
#include "Exceptions.hpp"

class Parser 
{
    private:
        unsigned int _port;
        std::string _psswrd;
    public:
        Parser();
        Parser( std::string port, std::string psswrd );
        ~Parser();

        void checkArguments( const std::string& port, const std::string& psswrd ) const;
        unsigned int getPort() const;
        std::string getPsswrd() const;
};

std::ostream &operator<<( std::ostream& os, const Parser& other );

#endif