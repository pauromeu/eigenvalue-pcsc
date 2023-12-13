#ifndef ABSTRACT_POWER_METHOD_H
#define ABSTRACT_POWER_METHOD_H

#include <Eigen/Dense>
#include <complex>
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

    /**
     * @brief Solve the eigenvalue problem.
     *
     * This method solves the eigenvalue problem using the abstract power method.
     * @throws IterationLimitExceeded if the iteration limit is exceeded before convergence.
     * @throws EigenvalueSolverNotConverged if the eigenvalue solver does not converge.
     * @return void
     */
    void solve()
    {
        this->initialize();
        currentIterations = 0;
        do
        {
            this->performIteration();
            ++currentIterations;
        } while (!this->hasConverged());
        this->obtainResults();
    }

    /**
     * @brief Get the eigenvalues.
     *
     * This method returns the eigenvalues of the matrix.
     * @return A vector containing the eigenvalues.
     */
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
    int currentIterations;
    Eigen::MatrixX<Scalar> currentVector;
    Eigen::MatrixX<Scalar> previousVector;
    Eigen::VectorX<Scalar> eigenvalues;
    Eigen::MatrixX<Scalar> eigenvectors;

    /**
     * @brief Check if the eigenvalue solver has converged.
     *
     * This method checks if the eigenvalue solver has converged.
     * @return A boolean indicating whether the eigenvalue solver has converged.
     * @throws IterationLimitExceeded if the iteration limit is exceeded before convergence.
     * @throws EigenvalueSolverNotConverged if the eigenvalue solver does not converge.
     */
    bool hasConverged() const
    {
        if (currentIterations >= maxIterations)
        {
            typename Eigen::NumTraits<Scalar>::Real diff = (previousVector - currentVector).norm();
            if (diff > tolerance)
            {
                throw IterationLimitExceeded("Iteration limit exceeded before convergence.");
            }
        }
        typename Eigen::NumTraits<Scalar>::Real diff = (previousVector - currentVector).norm();
        return diff < tolerance || currentIterations >= maxIterations;
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
