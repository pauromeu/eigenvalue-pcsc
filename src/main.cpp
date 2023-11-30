#include <iostream>
#include "Greeter.h"
#include "matrix_readers/MTXMatrixReader.h"

int main()
{
    // Ask name to user
    std::string name;
    std::cout << "Enter your name: ";
    std::cin >> name;
    Greeter greeter(name);
    std::cout << greeter.sayHello() << std::endl;

    // Aske user name of matrix to laod
    std::string matrixName;
    std::cout << "Enter matrix name: ";
    std::cin >> matrixName;

    // Load matrix
    MTXMatrixReader reader = MTXMatrixReader("data/matrix/" + matrixName + ".mtx");

    reader.readMatrix();

    auto matrix = reader.getDenseMatrix();

    // Print matrix
    std::cout << matrix << std::endl;
}
