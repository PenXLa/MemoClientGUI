#ifndef MEMOCLIENT_EXCEPTIONS_H
#define MEMOCLIENT_EXCEPTIONS_H

#include <exception>

struct unable_to_connect : public std::exception {
    const char * what () const throw () {
        return "unable to connect to server";
    }
};
struct  lost_connection : public std::exception {
    const char * what () const throw () {
        return "lost connection";
    }
};

struct  file_not_found : public std::exception {
    const char * what () const throw () {
        return "file not found";
    }
};


#endif //MEMOCLIENT_EXCEPTIONS_H
