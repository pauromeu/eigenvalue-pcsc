#ifndef CUSTOMEXCEPTIONS_H
#define CUSTOMEXCEPTIONS_H

#include <stdexcept>
#include <string>

class IterationLimitExceeded : public std::runtime_error {
public:
    IterationLimitExceeded(const std::string& message) : std::runtime_error(message) {}
};

class InvalidInputException : public std::runtime_error {
public:
    InvalidInputException(const std::string& message) : std::runtime_error(message) {}
};

#endif //CUSTOMEXCEPTIONS_H
