#include <algorithms/EigenvalueSolver.h>

#ifndef QRMETHOD_H
#define QRMETHOD_H

#include <iostream>
#include "EigenvalueSolver.h"
#include <Eigen/Dense>
#include <cmath>

template <typename Scalar>
class QRMethod : public EigenvalueSolver<Scalar>
{
public:
    QRMethod(int maxIterations = 100000, Scalar tol = 1e-6) : currentIteration(0), maxIterations(maxIterations), tolerance(tol) {}

    void setMatrix(const Eigen::MatrixX<Scalar> &matrix)
    {
        this->matrix = matrix;
    }
    void setMaxIterations(int maxIter)
    {
        this->maxIterations = maxIter;
    }
    void setTolerance(Scalar tol)
    {
        this->tolerance = tol;
    }

protected:
    void initialize()
    {
        currentIteration = 0;
    }

    bool hasConverged() const
    {
        if (currentIteration >= maxIterations)
        {
            std::cout << "Warning: QR method did not converge after " << maxIterations << " iterations." << std::endl;
            return true;
        }
        return false;
    }

    void performIteration()
    {

        Eigen::HouseholderQR<Eigen::MatrixX<Scalar>> qr = matrix.householderQr();
        Eigen::MatrixX<Scalar> Q = qr.householderQ();
        Eigen::MatrixX<Scalar> R = qr.matrixQR().template triangularView<Eigen::Upper>();
        matrix = R * Q;

        currentIteration++;
    }

    void obtainResults()
    {
        {
            int i = 0;
            int n = matrix.rows();
            Eigen::VectorX<std::complex<double>> tempEigenvalues;

            while (i < n)
            {
                if (i < n - 1 && std::abs(matrix(i + 1, i)) > std::abs(this->tolerance))
                {
                    auto block = matrix.block(i, i, 2, 2);
                    auto conjugateEigenvalues = computeComplexConjugateEigenvalues(block);
                    int currentSize = tempEigenvalues.size();
                    tempEigenvalues.conservativeResize(currentSize + 2);
                    tempEigenvalues(currentSize) = conjugateEigenvalues[0];
                    tempEigenvalues(currentSize + 1) = conjugateEigenvalues[1];
                    i += 2;
                }
                else
                {
                    int currentSize = tempEigenvalues.size();
                    tempEigenvalues.conservativeResize(currentSize + 1);
                    tempEigenvalues(currentSize) = std::complex<double>(matrix(i, i));
                    i += 1;
                }
            }

            // Assign the temporary eigenvalues to the class member
            this->eigenvalues = tempEigenvalues;
        }
    }

    Eigen::VectorX<std::complex<double>> computeComplexConjugateEigenvalues(const Eigen::MatrixX<Scalar> &block)
    {
        Scalar a = 1;                                                     // Coefficient of λ^2
        Scalar b = -(block(0, 0) + block(1, 1));                          // Coefficient of λ
        Scalar c = block(0, 0) * block(1, 1) - block(0, 1) * block(1, 0); // Constant term

        Scalar discriminant = b * b - Scalar(4) * a * c;

        Eigen::VectorX<std::complex<double>> roots(2);

        // Check if Scalar is a complex type
        if constexpr (std::is_same<Scalar, std::complex<float>>::value || std::is_same<Scalar, std::complex<double>>::value)
        {
            Scalar sqrtDiscriminant = std::sqrt(discriminant);
            roots[0] = std::complex<double>((-b + sqrtDiscriminant) / (Scalar(2) * a));
            roots[1] = std::complex<double>((-b - sqrtDiscriminant) / (Scalar(2) * a));
        }
        else // Handle real number types
        {
            std::complex<Scalar> complexDiscriminant = discriminant;
            std::complex<Scalar> sqrtDiscriminant = std::sqrt(complexDiscriminant);
            roots[0] = std::complex<double>((-b + sqrtDiscriminant) / (Scalar(2) * a));
            roots[1] = std::complex<double>((-b - sqrtDiscriminant) / (Scalar(2) * a));
        }

        return roots;
    }

private:
    Eigen::MatrixX<Scalar> matrix;
    int currentIteration;
    int maxIterations;
    Scalar tolerance;
};

template class QRMethod<double>;
template class QRMethod<float>;
template class QRMethod<std::complex<float>>;
template class QRMethod<std::complex<double>>;

#endif // QRMETHOD_H
