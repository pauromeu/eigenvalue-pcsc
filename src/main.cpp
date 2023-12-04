#include <iostream>
#include "Greeter.h"
#include "matrix_readers/MTXMatrixReader.h"
#include <algorithms/QRMethod.h>

int main()
{
    // Load matrix
    MTXMatrixReader reader = MTXMatrixReader("data/matrix/can_24.mtx");

    reader.readMatrix();

    Eigen::MatrixXd matrix = reader.getDenseMatrix();

    // Compute eigenvalues using QR method
    using Scalar = double;

    QRMethod<Scalar> qrMethod;

    qrMethod.setMatrix(matrix);

    qrMethod.solve();

    std::cout << "Eigenvalues QR: " << std::endl;
    std::cout << qrMethod.getEigenvalues().transpose() << std::endl;

    // Compute eigenvalues of the matrix with eigen to chek the results
    Eigen::EigenSolver<Eigen::MatrixXd> eigenSolver(matrix);

    std::cout << "Eigenvalues Eigen: " << std::endl;
    std::cout << eigenSolver.eigenvalues().transpose() << std::endl;
}
