#include <iostream>
#include "algorithms/QRMethod.h"

template <typename Scalar>
void QRMethod<Scalar>::setMatrix(const Eigen::MatrixX<Scalar> &matrix)
{
    this->matrix = matrix;
}

template <typename Scalar>
void QRMethod<Scalar>::initialize()
{
    currentIteration = 0;
}

template <typename Scalar>
void QRMethod<Scalar>::setMaxIterations(int maxIter)
{
    this->maxIterations = maxIter;
}

template <typename Scalar>
void QRMethod<Scalar>::setTolerance(Scalar tol)
{
    this->tolerance = tol;
}

template <typename Scalar>
bool QRMethod<Scalar>::hasConverged() const
{
    if (currentIteration >= maxIterations)
    {
        std::cout << "Warning: QR method did not converge after " << maxIterations << " iterations." << std::endl;
        return true;
    }

    for (int i = 0; i < matrix.rows() - 1; ++i)
    {
        if (std::abs(matrix(i + 1, i)) > std::abs(tolerance))
        {
            return false;
        }
    }
    std::cout << "QR method converged after " << currentIteration << " iterations." << std::endl;
    return true;
}

template <typename Scalar>
void QRMethod<Scalar>::performIteration()
{
    Eigen::HouseholderQR<Eigen::MatrixX<Scalar>> qr = matrix.householderQr();
    Eigen::MatrixX<Scalar> Q = qr.householderQ();
    Eigen::MatrixX<Scalar> R = qr.matrixQR().template triangularView<Eigen::Upper>();
    matrix = R * Q;

    currentIteration++;
}

template <typename Scalar>
void QRMethod<Scalar>::obtainResults()
{
    this->eigenvalues = matrix.diagonal().template cast<std::complex<double>>();
}
