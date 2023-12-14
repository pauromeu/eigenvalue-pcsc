#ifndef EIGENVALUE_SOLVER_H
#define EIGENVALUE_SOLVER_H

#include <Eigen/Dense>
#include <complex>
#include <type_traits>

template <typename Scalar>
class EigenvalueSolver
{
public:
    static_assert(std::is_floating_point<Scalar>::value || std::is_same<Scalar, std::complex<float>>::value || std::is_same<Scalar, std::complex<double>>::value,
                  "Scalar must be a floating point or complex type");

    virtual ~EigenvalueSolver() {}

    virtual void setMatrix(const Eigen::MatrixX<Scalar> &matrix) = 0;
    virtual void setMaxIterations(int maxIter) = 0;
    virtual void setTolerance(Scalar tol) = 0;

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
    //Eigen::MatrixX<Scalar> getEigenvectors() const { return eigenvectors; }
    virtual Eigen::MatrixX<Scalar> getEigenvectors() const = 0;

protected:
    virtual void initialize() = 0;
    virtual bool hasConverged() = 0;
    virtual void performIteration() = 0;
    virtual void obtainResults() = 0;

    bool converged;
    Eigen::VectorX<std::complex<double>> eigenvalues;
    Eigen::MatrixX<Scalar> eigenvectors;

    int currentIteration;
};

#endif // EIGENVALUE_SOLVER_H