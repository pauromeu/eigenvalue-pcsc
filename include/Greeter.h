#ifndef GREETER_H
#define GREETER_H

#include <string>

/**
 * @class Greeter
 * @brief A class for greeting purposes.
 *
 * Greeter generates a greeting message that includes a specified name.
 */
class Greeter
{
public:
    /**
     * @brief Construct a new Greeter object.
     *
     * Initializes the Greeter with a given name.
     * @param name The name to be used in the greeting.
     */
    Greeter(const std::string &name);

    /**
     * @brief Generate a greeting message.
     *
     * Creates and returns a greeting message.
     * @return A string containing the greeting message.
     */
    std::string sayHello() const;

private:
    std::string name; ///< The name to be used in the greeting.
};

#endif // GREETER_H
