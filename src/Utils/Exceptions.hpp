#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>

class WrongPortSizeException : public std::exception {
public:
    virtual const char *what() const throw() {
        return "ERROR: PORT SIZE MUST BE BETWEEN 1 AND 5";
    }
};

class WrongDigitPortException : public std::exception {
public:
    virtual const char *what() const throw() {
        return "ERROR: PORT CAN ONLY CONTAIN DIGITS";
    }
};

class WrongPasswordException : public std::exception {
public:
    virtual const char *what() const throw() {
        return "ERROR: PASSWORD IS WRONG";
    }
};

class SocketCreationFailedException : public std::exception {
public:
    virtual const char *what() const throw() {
        return "ERROR: SOCKET FAILED TO CREATE";
    }
};

#endif