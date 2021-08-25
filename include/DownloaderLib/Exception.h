//
// Created by cysiu on 30.10.2019.
//

#ifndef LIBRARIES1_EXCEPTION_H
#define LIBRARIES1_EXCEPTION_H
#include <exception>
#include <string>

using namespace std;

class Exception : public exception
{
    string _msg;
public:
    Exception(const string& msg) : _msg(msg){}

    virtual const char* what() const noexcept override
    {
        return _msg.c_str();
    }

    virtual string msg() const noexcept
    {
        return _msg;
    }
};

#endif //LIBRARIES1_EXCEPTION_H
