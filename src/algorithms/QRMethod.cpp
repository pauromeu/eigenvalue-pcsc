#include "algorithms/QRMethod.h"
#include <Eigen/Dense>

template <typename Scalar>
QRMethod<Scalar>::QRMethod(int maxIterations, Scalar tol)
    : currentIteration(0), tolerance(tol)
{
    this->maxIterations = maxIterations;
}

template <typename Scalar>
void QRMethod<Scalar>::setMatrix(const Eigen::MatrixX<Scalar> &mat)
{
    matrix = mat;
    this->eigenvalues.resize(matrix.rows());
}

template <typename Scalar>
void QRMethod<Scalar>::solve()
{
    if (matrix.rows() != matrix.cols())
    {
        throw std::runtime_error("QR Method requires a square matrix.");
    }
    initialize();
    while (!hasConverged() && currentIteration < this->maxIterations)
    {
        performIteration();
    }
}

template <typename Scalar>
bool QRMethod<Scalar>::hasConverged() const
{
    // Implement a convergence check. For example, check if the off-diagonal elements are close to zero.
    // Placeholder for convergence check

    return false;
}

template <typename Scalar>
void QRMethod<Scalar>::performIteration()
{
    Eigen::HouseholderQR<Eigen::MatrixX<Scalar>> qr(matrix);
    matrix = qr.matrixR() * qr.householderQ();
    currentIteration++;
}

template <typename Scalar>
void QRMethod<Scalar>::initialize()
{
    currentIteration = 0;
}

// Explicit template instantiation for double and std::complex<double>
template class QRMethod<double>;
template class QRMethod<std::complex<double>>;
