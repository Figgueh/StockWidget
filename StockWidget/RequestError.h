#pragma once
#include <exception>

class RequestError : public std::exception
{
public:
    RequestError() = default;
    const char* what() const throw();
};