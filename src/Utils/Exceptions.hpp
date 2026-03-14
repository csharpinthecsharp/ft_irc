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

class ServerSocketCreationFailedException : public std::exception {
public:
    virtual const char *what() const throw() {
        return "ERROR: SERVER SOCKET FAILED TO CREATE";
    }
};

class ClientSocketCreationFailedException : public std::exception {
public:
    virtual const char *what() const throw() {
        return "ERROR: CLIENT SOCKET FAILED TO CREATE";
    }
};

class BindFailedExpection : public std::exception {
public:
    virtual const char *what() const throw() {
        return "ERROR: BIND FAILED";
    }
};

class ListenFailedExpection : public std::exception {
public:
    virtual const char *what() const throw() {
        return "ERROR: LISTENING FAILED";
    }
};

class GettingNameInfoFailedException : public std::exception {
public:
    virtual const char *what() const throw() {
        return "ERROR: CANNOT RETRIEVE NAME INFO";
    }
};

#endif