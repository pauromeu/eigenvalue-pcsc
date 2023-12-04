#ifndef POWER_METHOD_H
#define POWER_METHOD_H

#include "AbstractPowerMethod.h"

template <typename Scalar>
class PowerMethod : public AbstractPowerMethod<Scalar>
{
public:
    using AbstractPowerMethod<Scalar>::AbstractPowerMethod;

    void setMatrix(const Eigen::MatrixX<Scalar>& mat) {
        matrix = mat;
    }

    void setTolerance(Scalar tol) {
        tolerance = tol;
    }

    void setMaxIterations(int maxIter) {
        maxIterations = maxIter;
    }

    void solve() {
        this->initialize();
        currentIterations = 0;

        do {
            this->performIteration();
            ++currentIterations;
        } while (!this->hasConverged());

        this->obtainResults();
    }

    Eigen::VectorX<Scalar> getEigenvalues() const {
        return eigenvalues;
    }

    Eigen::MatrixX<Scalar> getEigenvectors() const {
        return eigenvectors;
    }

protected:
    bool hasConverged() const {
        Scalar diff = (previousVector - currentVector).norm();
        return diff < tolerance || currentIterations >= maxIterations;
    }

    void performIteration() {
        previousVector = currentVector;
        currentVector = matrix * currentVector;
        currentVector.normalize();
    }

    void initialize() {
        currentVector = Eigen::MatrixX<Scalar>::Random(matrix.rows(), 1);
        previousVector = currentVector;
    }

    void obtainResults() {
        Eigen::Matrix<Scalar, Eigen::Dynamic, 1> vectorizedCurrentVector = currentVector;
        Scalar eigenvalue = (matrix * vectorizedCurrentVector).dot(vectorizedCurrentVector) / vectorizedCurrentVector.squaredNorm();

        this->eigenvalues = Eigen::VectorX<Scalar>::Constant(1, eigenvalue);
        this->eigenvectors = vectorizedCurrentVector;
    }

private:
    Eigen::MatrixX<Scalar> matrix;
    Eigen::MatrixX<Scalar> currentVector;
    Eigen::MatrixX<Scalar> previousVector;
    Eigen::VectorX<Scalar> eigenvalues;
    Eigen::MatrixX<Scalar> eigenvectors;
    int maxIterations;
    int currentIterations;
    Scalar tolerance;
};

#endif // POWER_METHOD_H
