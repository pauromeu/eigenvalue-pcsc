#ifndef EIGENVALUE_SOLVER_H
#define EIGENVALUE_SOLVER_H

#include <Eigen/Dense>
#include <complex>

template <typename Scalar>
class EigenvalueSolver
{
public:
    virtual ~EigenvalueSolver() {}

    virtual void setMatrix(const Eigen::MatrixX<Scalar> &matrix) = 0;
    virtual void setMaxIterations(int maxIter) = 0;
    virtual void setTolerance(Scalar tol) = 0;

    virtual void solve() = 0;

    Eigen::VectorX<Scalar> getEigenvalues() const { return eigenvalues; }
    Eigen::MatrixX<Scalar> getEigenvectors() const { return eigenvectors; }

protected:
    virtual bool hasConverged() const = 0;
    virtual void performIteration() = 0;
    virtual void initialize() = 0;

private:
    Eigen::MatrixX<Scalar> matrix;
    Eigen::VectorX<Scalar> eigenvalues;
    Eigen::MatrixX<Scalar> eigenvectors;
    int maxIterations;
    Scalar tolerance;
};

#endif // EIGENVALUE_SOLVER_H
