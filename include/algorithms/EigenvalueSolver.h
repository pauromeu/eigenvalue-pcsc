#ifndef EIGENVALUE_SOLVER_H
#define EIGENVALUE_SOLVER_H

#include <Eigen/Dense>
#include <complex>
#include <type_traits>

/**
 * @brief Eigenvalue solver base class
 * This class is the base class for all eigenvalue solvers.
 * It provides the interface for all eigenvalue solvers.
 * The solve() function is the main function that should be called to solve the eigenvalue problem.
 * The getEigenvalues() and getEigenvectors() functions can be called after the solve() function to obtain the results.
 *
 * @tparam Scalar The scalar type of the matrix to solve the eigenvalue problem for.
 * This can be one of float, double, std::complex<float>, or std::complex<double>.
 *
 * @note This class is not meant to be used directly. Use one of the derived classes instead.
 */
template <typename Scalar>
class EigenvalueSolver
{
public:
    static_assert(std::is_floating_point<Scalar>::value || std::is_same<Scalar, std::complex<float>>::value || std::is_same<Scalar, std::complex<double>>::value,
                  "Scalar must be a floating point or complex type");

    /**
     * @brief Destroy the Eigenvalue Solver object
     *
     */
    virtual ~EigenvalueSolver() {}

    /**
     * @brief Set the matrix to solve the eigenvalue problem for.
     *
     * @param matrix The matrix to solve the eigenvalue problem for.
     */
    virtual void setMatrix(const Eigen::MatrixX<Scalar> &matrix) = 0;

    /**
     * @brief Set the maximum number of iterations to perform.
     * This function sets the maximum number of iterations to perform before the solver stops.
     * The solver will stop if it has converged before the maximum number of iterations is reached.
     *
     * @param maxIter The maximum number of iterations to perform
     */
    virtual void setMaxIterations(int maxIter) = 0;

    /**
     * @brief Set the tolerance for the solver.
     * This function sets the tolerance for the solver. The solver will stop when the residual is less than the tolerance.
     *
     * @param tol The tolerance for the solver
     */
    virtual void setTolerance(Scalar tol) = 0;

    /**
     * @brief Solve the eigenvalue problem
     * This function solves the eigenvalue problem using the iterative method implemented by the solver.
     * It will perform iterations until the solver has converged to the desired tolerance.
     * The results can be obtained using the getEigenvalues() and getEigenvectors() functions.
     */
    void solve()
    {
        this->initialize();
        currentIteration = 0;
        do
        {
            this->performIteration();
            ++currentIteration;
        } while (!this->hasConverged());
        this->obtainResults();
    }

    virtual Eigen::VectorX<std::complex<double>> getEigenvalues() const { return eigenvalues; }
    // Eigen::MatrixX<Scalar> getEigenvectors() const { return eigenvectors; }
    virtual Eigen::MatrixX<Scalar> getEigenvectors() const = 0;

protected:
    /**
     * @brief Initialize the solver
     * Perform any variable initialization here
     */
    virtual void initialize() = 0;
  
     /**
     * @brief Check if the solver has converged
     * Using the current iteration and the tolerance, check if the solver has converged to the desired tolerance
     *
     * @return true if the solver has converged, false otherwise
     */
    virtual bool hasConverged() = 0;

    /**
     * @brief Perform a single iteration of the solver
     * All solver are iterative. This function performs a single iteration of the solver.
     */

    virtual void performIteration() = 0;

    /**
     * @brief Obtain the results of the solver
     * This function is to be called after the solver has converged. It obtains the eigenvalues and eigenvectors
     * from the solver and store them in the appropriate member variables.
     */
    virtual void obtainResults() = 0;

    bool converged;
    Eigen::VectorX<std::complex<double>> eigenvalues;
    Eigen::MatrixX<Scalar> eigenvectors;

    int currentIteration;
};

#endif // EIGENVALUE_SOLVER_H