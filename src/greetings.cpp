#include "greetings.h"

/**
 * Implements the function to generate a greeting message.
 *
 * @param name The name of the person to greet.
 * @return A string containing the greeting message.
 */
std::string getGreeting(const std::string &name)
{
    return "Hello, " + name + "!";
}
