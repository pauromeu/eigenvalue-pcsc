#ifndef ABSTRACT_POWER_METHOD_H
#define ABSTRACT_POWER_METHOD_H

#include <Eigen/Dense>
#include <complex>
#include "EigenvalueSolver.h"
#include "exceptions/CustomExceptions.h"

template <typename Scalar>
class AbstractPowerMethod : public EigenvalueSolver<Scalar>
{
public:
    using EigenvalueSolver<Scalar>::EigenvalueSolver;

    void solve() {
        this->initialize();
        currentIterations = 0;
        do {
            this->performIteration();
            ++currentIterations;
        } while (!this->hasConverged());
        if (currentIterations >= this->maxIterations && ((previousVector - currentVector).norm() >= tolerance)) {
            Scalar diff = (previousVector - currentVector).norm();
            throw IterationLimitExceeded("Iteration limit exceeded before convergence.");
        }

        this->obtainResults();
    }

    Eigen::VectorX<Scalar> getEigenvalues() const {
        return eigenvalues;
    }

    Eigen::MatrixX<Scalar> getEigenvectors() const {
        return eigenvectors;
    }

    virtual void setMatrix(const Eigen::MatrixX<Scalar>& mat) = 0;

    void setTolerance(Scalar tol) {
        tolerance = tol;
    }

    void setMaxIterations(int maxIter) {
        maxIterations = maxIter;
    }

protected:
    Eigen::MatrixX<Scalar> matrix;
    int maxIterations;
    Scalar tolerance;
    int currentIterations;
    Eigen::MatrixX<Scalar> currentVector;
    Eigen::MatrixX<Scalar> previousVector;
    Eigen::VectorX<Scalar> eigenvalues;
    Eigen::MatrixX<Scalar> eigenvectors;

    bool hasConverged() const {
        Scalar diff = (previousVector - currentVector).norm();
        return diff < tolerance || currentIterations >= maxIterations;
    }

    virtual void performIteration() = 0;

    void initialize() {
        currentVector = Eigen::MatrixX<Scalar>::Random(matrix.rows(), 1);
        previousVector = currentVector;
    }

    virtual void obtainResults() override = 0;


};

#endif // ABSTRACT_POWER_METHOD_H

