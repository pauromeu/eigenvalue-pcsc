#ifndef ABSTRACT_POWER_METHOD_H
#define ABSTRACT_POWER_METHOD_H

#include <Eigen/Dense>
#include <complex>
#include <iostream>
#include "EigenvalueSolver.h"
#include "exceptions/CustomExceptions.h"


/**
 * @class AbstractPowerMethod
 * @brief A class for solving eigenvalue problems using the power method.
 *
 * @tparam Scalar
 * AbstractPowerMethod is an abstract class that provides the basic structure
 */

template <typename Scalar>
class AbstractPowerMethod : public EigenvalueSolver<Scalar>
{
public:
    using EigenvalueSolver<Scalar>::EigenvalueSolver;

    Eigen::VectorX<std::complex<double>> getEigenvalues() const override
    {
        return eigenvalues.template cast<std::complex<double>>();
    }


    /**
     * @brief Get the eigenvectors.
     *
     * This method returns the eigenvectors of the matrix.
    */
    Eigen::MatrixX<Scalar> getEigenvectors() const
    {
        return eigenvectors;
    }

    /**
     * @brief Set the matrix.
     *
     * This method sets the matrix to be used in the eigenvalue problem.
     * @param mat The matrix to be used in the eigenvalue problem.
     * @return void
     */
    virtual void setMatrix(const Eigen::MatrixX<Scalar> &mat) = 0;

    /**
     * @brief Set the tolerance.
     *
     * This method sets the tolerance to be used in the eigenvalue problem.
     */
    void setTolerance(Scalar tol) override
    {
        typename Eigen::NumTraits<Scalar>::Real realTolerance = std::abs(tol);
        tolerance = realTolerance;
    }

    /**
     * @brief Set the maximum number of iterations.
     *
     * This method sets the maximum number of iterations to be used in the eigenvalue problem.
     */
    void setMaxIterations(int maxIter)
    {
        maxIterations = maxIter;
    }

protected:
    Eigen::MatrixX<Scalar> matrix;
    int maxIterations;
    typename Eigen::NumTraits<Scalar>::Real tolerance;
    Eigen::MatrixX<Scalar> currentVector;
    Eigen::MatrixX<Scalar> previousVector;
    Eigen::VectorX<Scalar> eigenvalues;
    Eigen::MatrixX<Scalar> eigenvectors;

    bool check_near(const double &a, const double &b, const double &tolerance)
    {
        return std::abs(a - b) < tolerance;
    }

    bool checkEigenVector() {
        if constexpr (std::is_same<Scalar, std::complex<double>>::value || std::is_same<Scalar, std::complex<float>>::value) {
            // For complex number types
            std::complex<double> firstEigenvalue = eigenvalues(0); // Use eigenvalues directly
            Eigen::VectorX<Scalar> actualVector = eigenvectors.col(0);
            Eigen::VectorX<Scalar> result1 = matrix * actualVector;
            Eigen::VectorX<Scalar> result2 = firstEigenvalue * actualVector;

            for (int i = 0; i < result1.size(); ++i) {
                if (!check_near(result1(i).real(), result2(i).real(), tolerance) || !check_near(result1(i).imag(), result2(i).imag(), tolerance)) {
                    return false;
                }
            }
        } else {
            // For real number types
            Scalar firstEigenvalue = eigenvalues(0); // Use eigenvalues directly
            Eigen::VectorX<Scalar> actualVector = eigenvectors.col(0);
            Eigen::VectorX<Scalar> result1 = matrix * actualVector;
            Eigen::VectorX<Scalar> result2 = firstEigenvalue * actualVector;

            for (int i = 0; i < result1.size(); ++i) {
                if (!check_near(result1(i), result2(i), tolerance)) {
                    return false;
                }
            }
        }

        return true;
    }


    /**
     * @brief Check if the eigenvalue solver has converged.
     *
     * This method checks if the eigenvalue solver has converged.
     * @return A boolean indicating whether the eigenvalue solver has converged.
     * @throws IterationLimitExceeded if the iteration limit is exceeded before convergence.
     * @throws EigenvalueSolverNotConverged if the eigenvalue solver does not converge.
     */
    bool hasConverged()
    {
        if (this->currentIteration >= maxIterations)
        {
            this->obtainResults();
            typename Eigen::NumTraits<Scalar>::Real diff = (previousVector - currentVector).norm();
            bool isEigenVector = checkEigenVector();
            if (diff > tolerance && !isEigenVector)
            {
                std::cout<<"Algorithm did not converge"<<std::endl;
                std::cout<<"last diff was norm of: ("<<previousVector<<" - "<<currentVector<<") = "<<diff<<std::endl;
                std::cout<<"tolerance is: "<<tolerance<<std::endl;
                std::cout<<"Non-convergence result:\n------------------------\n";
                std::cout<<"Eigenvalue: "<<this->eigenvalues<<std::endl;
                std::cout<<"Eigenvector: "<<this->eigenvectors<<std::endl;
                throw IterationLimitExceeded("Iteration limit exceeded before convergence.");
            }
        }
        typename Eigen::NumTraits<Scalar>::Real diff = (previousVector - currentVector).norm();
        return diff < tolerance || this->currentIteration >= maxIterations;
    }

    virtual void performIteration() = 0;

    /**
     * @brief Initialize the eigenvalue solver.
     *
     * This method initializes the eigenvalue solver.
     */
    void initialize()
    {
        currentVector = Eigen::MatrixX<Scalar>::Random(matrix.rows(), 1);
        previousVector = currentVector;
    }

    /**
     * @brief Obtain the results of the eigenvalue solver.
     *
     * Virtual method to be implemented by the subclasses.
     */
    virtual void obtainResults() override = 0;
};

#endif // ABSTRACT_POWER_METHOD_H
