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

        typename Eigen::NumTraits<Scalar>::Real diff = (previousVector - currentVector).norm();
        if (currentIterations >= this->maxIterations && diff > this->tolerance){
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

    void setTolerance(Scalar tol) override {
        typename Eigen::NumTraits<Scalar>::Real realTolerance = std::abs(tol);
        tolerance = realTolerance;
    }

    void setMaxIterations(int maxIter) {
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

    bool hasConverged() const {
        typename Eigen::NumTraits<Scalar>::Real diff = (previousVector - currentVector).norm();
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

