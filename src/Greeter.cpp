#include "Greeter.h"

Greeter::Greeter(const std::string& name) : name(name) {}

std::string Greeter::sayHello() const {
    return "Hello, " + name + "!";
}
