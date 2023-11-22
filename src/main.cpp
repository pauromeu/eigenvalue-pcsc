#include <iostream>
#include "greetings.h"

int main()
{
    std::string name;
    std::cout << "Enter your name: ";
    std::getline(std::cin, name);

    std::cout << getGreeting(name) << std::endl;
    return 0;
}
