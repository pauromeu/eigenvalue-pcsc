#include <iostream>
#include "Greeter.h"
// #include "matrix_readers/MTXMatrixReader.h"
#include <algorithms/QRMethod.h>
#include <unistd.h>
#include <limits.h>

int main()
{
    // Load matrix
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        std::cout << "Current working dir: " << cwd << std::endl;
    }
    else
    {
        perror("getcwd() error");
        return 1;
    }

    // MTXMatrixReader reader = MTXMatrixReader("data/matrix/can_24.mtx");
    // reader.readMatrix();
    //  Eigen::MatrixXd matrix = reader.getDenseMatrix();

    // Create a 2x2 matrix
    Eigen::Matrix2cd matrix; // 'cd' stands for complex<double>

    // Initialize the matrix with complex numbers
    matrix(0, 0) = std::complex<double>(1, 2);  // 1 + 2i
    matrix(0, 1) = std::complex<double>(3, -4); // 3 - 4i
    matrix(1, 0) = std::complex<double>(-2, 1); // -2 + 1i
    matrix(1, 1) = std::complex<double>(5, 6);  // 5 + 6i

    // Compute eigenvalues using QR method

    using Scalar = std::complex<double>;
    QRMethod<Scalar> qrMethod;
    qrMethod.setMatrix(matrix);
    qrMethod.solve();
    std::cout << "Eigenvalues QR: " << std::endl;
    std::cout << qrMethod.getEigenvalues().transpose() << std::endl;

    // Compute eigenvalues of the matrix with eigen to chek the results
    // Eigen::EigenSolver<Eigen::MatrixXd> eigenSolver(matrix);

    std::cout << "Eigenvalues Eigen: " << std::endl;
    // std::cout << eigenSolver.eigenvalues().transpose() << std::endl;
}
