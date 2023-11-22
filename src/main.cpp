#include <iostream>
#include "Greeter.h"

int main()
{
    std::string name;
    std::cout << "Enter your name: ";
    std::getline(std::cin, name);

    Greeter greeter(name);
    std::cout << greeter.sayHello() << std::endl;

    return 0;
}
