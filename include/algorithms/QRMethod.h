#ifndef QRMETHOD_H
#define QRMETHOD_H

#include <algorithms/EigenvalueSolver.h>
#include <iostream>
#include "EigenvalueSolver.h"
#include <Eigen/Dense>
#include <cmath>
#include <exceptions/SolverException.h>

/**
 * @brief A class for solving the eigenvalue problem using the QR method.
 *
 * The QR method is an iterative method for computing the eigenvalues of a matrix. It is based on the QR decomposition of the matrix.
 * By repeatedly computing the QR decomposition of the matrix, the matrix converges to a triangular matrix with the eigenvalues on the diagonal.
 *
 * In order to support complex eigenvalues, the QR method uses a 2x2 block of the matrix to compute the eigenvalues using the quadratic formula. In this case, the matrix is not triangular.
 *
 * The QR method does not support computing the eigenvectors.
 *
 * @tparam Scalar The scalar type of the matrix. It can be either double or std::complex<double>.
 */
template <typename Scalar>
class QRMethod : public EigenvalueSolver<Scalar>
{
public:
    /**
     * @brief Construct a new QR Method object
     *
     * @param maxIterations The maximum number of iterations to perform
     * @param tol The tolerance for the solver
     * @return void
     */
    QRMethod(int maxIterations = 100000, Scalar tol = 1e-6) : maxIterations(maxIterations), tolerance(tol) {}

    /**
     * @brief Get the eigenvectors.
     *
     * @return The eigenvectors of the matrix
     *
     * @note This function throws a NotImplementedSolverException because the QR method does not support computing the eigenvectors.
     */
    Eigen::MatrixX<Scalar> getEigenvectors() const override
    {
        throw NotImplementedSolverException("Eigenvectors are not supported for the QR method.",
                                            "Use a different method to obtain the eigenvectors.");
    }

    /**
     * @brief Set the matrix to solve the eigenvalue problem for.
     *
     * @param matrix The matrix to solve the eigenvalue problem for.
     */
    void setMatrix(const Eigen::MatrixX<Scalar> &matrix)
    {
        this->matrix = matrix;
    }

    /**
     * @brief Set the maximum number of iterations to perform.
     * This function sets the maximum number of iterations to perform before the solver stops.
     * The solver will stop if it has converged before the maximum number of iterations is reached.
     *
     * @param maxIter The maximum number of iterations to perform
     */
    void setMaxIterations(int maxIter)
    {
        this->maxIterations = maxIter;
    }

    /**
     * @brief Set the tolerance for the solver.
     * This function sets the tolerance for the solver. The solver will stop when the residual is less than the tolerance.
     *
     * @param tol The tolerance for the solver
     */
    void setTolerance(Scalar tol)
    {
        this->tolerance = tol;
    }

protected:
    /**
     * @brief Initialize the solver.
     *
     * This function initializes the solver. It is called before the first iteration.
     *
     * @note This function does nothing for the QR method.
     */
    void initialize()
    {
    }

    /**
     * @brief Check if the solver has converged.
     *
     * This function checks if the solver has converged to the desired tolerance.
     *
     * @return True if the solver has converged, false otherwise.
     */
    bool hasConverged()

    {
        if (this->currentIteration >= maxIterations)
        {
            // std::cout << "Warning: QR method did not converge after " << maxIterations << " iterations." << std::endl;
            return true;
        }
        return false;
    }

    /**
     * @brief Perform an iteration of the QR method.
     *
     * This function performs an iteration of the QR method.
     * First, the QR decomposition of the current matrix is computed.
     * Then, the matrix is updated to be the product of the R and Q matrices.
     * This process is supposed to converge to a triangular matrix with the eigenvalues on the diagonal.
     */
    void performIteration()
    {
        Eigen::HouseholderQR<Eigen::MatrixX<Scalar>> qr = matrix.householderQr();
        Eigen::MatrixX<Scalar> Q = qr.householderQ();
        Eigen::MatrixX<Scalar> R = qr.matrixQR().template triangularView<Eigen::Upper>();
        matrix = R * Q;
    }

    /**
     * @brief Obtain the results of the QR method.
     *
     * This function obtains the results of the QR method.
     * It first obtains the eigenvalues of the matrix.
     * Then, it sorts the eigenvalues in descending order of their absolute values.
     * Finally, it assigns the eigenvalues to the class member.
     */
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

            // Sort the eigenvalues in descending order of their absolute values
            std::sort(tempEigenvalues.data(), tempEigenvalues.data() + tempEigenvalues.size(), [](const std::complex<double> &a, const std::complex<double> &b)
                      { return std::abs(a) > std::abs(b); });

            // Assign the temporary eigenvalues to the class member
            this->eigenvalues = tempEigenvalues;
        }
    }

    /**
     * @brief Compute the complex conjugate eigenvalues of a 2x2 matrix.
     *
     * This function computes the complex conjugate eigenvalues of a 2x2 matrix.
     * It uses the quadratic formula to compute the eigenvalues.
     *
     * @param block The 2x2 matrix to compute the eigenvalues of.
     * @return The complex conjugate eigenvalues of the matrix.
     */
    Eigen::VectorX<std::complex<double>> computeComplexConjugateEigenvalues(const Eigen::MatrixX<Scalar> &block)
    {
        // Compute the coefficients of the quadratic equation
        Scalar a = 1;                                                     // Coefficient of λ^2
        Scalar b = -(block(0, 0) + block(1, 1));                          // Coefficient of λ
        Scalar c = block(0, 0) * block(1, 1) - block(0, 1) * block(1, 0); // Constant term

        // Compute the discriminant
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
    int maxIterations;
    Scalar tolerance;
};

template class QRMethod<double>;
template class QRMethod<float>;
template class QRMethod<std::complex<float>>;
template class QRMethod<std::complex<double>>;

#endif // QRMETHOD_H
