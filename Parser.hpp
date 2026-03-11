#ifndef PARSER_HPP
#define PARSER_HPP

#define PORT_MAX_SIZE 5
#define PORT_MIN_SIZE 1

#include <iostream>
#include <sstream>
#include <exception>

class Parser {
    private:
        unsigned int _port;
        std::string _psswrd;
    public:
        Parser();
        Parser( std::string port, std::string psswrd );
        ~Parser();

        unsigned int getPort() const;
        std::string getPsswrd() const;

        class WrongPortSizeException : public std::exception {
            virtual const char *what() const throw() {
                return "ERROR: PORT SIZE MUST BE BETWEEN 1 AND 5";
            }
        };
        class WrongDigitPortException : public std::exception {
            virtual const char *what() const throw() {
                return "ERROR: PORT CAN ONLY CONTAIN DIGITS";
            }
        };
};

std::ostream &operator<<( std::ostream& os, const Parser& other );

#endif