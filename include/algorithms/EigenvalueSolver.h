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
        initialize();
        while (!hasConverged())
        {
            performIteration();
        }
        obtainResults();
    }

    virtual Eigen::VectorX<std::complex<double>> getEigenvalues() const { return eigenvalues; }
    Eigen::MatrixX<Scalar> getEigenvectors() const { return eigenvectors; }

protected:
    virtual void initialize() = 0;
    virtual bool hasConverged() const = 0;
    virtual void performIteration() = 0;
    virtual void obtainResults() = 0;

    Eigen::VectorX<std::complex<double>> eigenvalues;
    Eigen::MatrixX<Scalar> eigenvectors;
};

#endif // EIGENVALUE_SOLVER_H