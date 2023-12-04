#ifndef INVERSE_POWER_METHOD_WITH_SHIFT_H
#define INVERSE_POWER_METHOD_WITH_SHIFT_H

#include "AbstractPowerMethod.h"

template <typename Scalar>
class InversePowerMethodWithShift : public AbstractPowerMethod<Scalar>
{
public:
    using AbstractPowerMethod<Scalar>::AbstractPowerMethod;

    void setMatrix(const Eigen::MatrixX<Scalar>& mat) {
        matrix = mat;
        shiftedMatrix = matrix - shift * Eigen::MatrixX<Scalar>::Identity(matrix.rows(), matrix.cols());
        matrixInverse = shiftedMatrix.inverse();
    }

    void setTolerance(Scalar tol) {
        tolerance = tol;
    }

    void setMaxIterations(int maxIter) {
        maxIterations = maxIter;
    }

    void setShift(Scalar sh) {
        shift = sh;
    }

    void solve() {
        this->initialize();
        currentIterations = 0;
        do {
            this->performIteration();
            ++currentIterations;
        } while (!this->hasConverged());

        this->ObtainResults();
    }

    Eigen::VectorX<Scalar> getEigenvalues() const  {
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
        currentVector = matrixInverse * currentVector;
        currentVector.normalize();
    }

    void initialize()  {
        currentVector = Eigen::MatrixX<Scalar>::Random(matrix.rows(), 1);
        previousVector = currentVector;
    }

    void ObtainResults() {
        Eigen::Matrix<Scalar, Eigen::Dynamic, 1> vectorizedCurrentVector = currentVector;
        Scalar eigenvalue = (matrixInverse * vectorizedCurrentVector).dot(vectorizedCurrentVector) / vectorizedCurrentVector.squaredNorm();

        this->eigenvalues = Eigen::VectorX<Scalar>::Constant(1, 1.0 / eigenvalue + shift);
        this->eigenvectors = vectorizedCurrentVector;
    }


private:
    Eigen::MatrixX<Scalar> matrix;
    Eigen::MatrixX<Scalar> matrixInverse;
    Eigen::MatrixX<Scalar> shiftedMatrix;
    Eigen::MatrixX<Scalar> currentVector;
    Eigen::MatrixX<Scalar> previousVector;
    Eigen::VectorX<Scalar> eigenvalues;
    Eigen::MatrixX<Scalar> eigenvectors;
    int maxIterations;
    int currentIterations;
    Scalar shift;
    Scalar tolerance;
};

#endif // INVERSE_POWER_METHOD_WITH_SHIFT_H
