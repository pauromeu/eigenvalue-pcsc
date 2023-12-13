#ifndef CUSTOMEXCEPTIONS_H
#define CUSTOMEXCEPTIONS_H

#include <stdexcept>
#include <string>

class IterationLimitExceeded : public std::runtime_error {
public:
    /**
     * @brief Construct a new IterationLimitExceeded object.
     * This method constructs a new IterationLimitExceeded object with a given message.
     * @param message The message to be used in the exception.
     * @return A new IterationLimitExceeded object.
     * */
    IterationLimitExceeded(const std::string& message) : std::runtime_error(message) {}
};

class InvalidInputException : public std::runtime_error {
public:
    /**
     * @brief Construct a new InvalidInputException object.
     * This method constructs a new InvalidInputException object with a given message.
     * @param message The message to be used in the exception.
     * @return A new InvalidInputException object.
     * */
    InvalidInputException(const std::string& message) : std::runtime_error(message) {}
};

#endif //CUSTOMEXCEPTIONS_H
