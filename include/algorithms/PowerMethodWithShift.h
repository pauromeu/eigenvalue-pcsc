#ifndef POWER_METHOD_WITH_SHIFT_H
#define POWER_METHOD_WITH_SHIFT_H

#include "AbstractPowerMethod.h"

template <typename Scalar>
class PowerMethodWithShift : public AbstractPowerMethod<Scalar>
{
public:
    using AbstractPowerMethod<Scalar>::AbstractPowerMethod;

    void setMatrix(const Eigen::MatrixX<Scalar>& mat) override {
        matrix = mat;
        shiftedMatrix = matrix - shift * Eigen::MatrixX<Scalar>::Identity(matrix.rows(), matrix.cols());
    }

    void setTolerance(Scalar tol) override {
        tolerance = tol;
    }

    void setMaxIterations(int maxIter) override {
        maxIterations = maxIter;
    }

    void setShift(Scalar sh) {
        shift = sh;
    }

    void solve() override {
        this->initialize();
        currentIterations = 0;

        do {
            this->performIteration();
            ++currentIterations;
        } while (!this->hasConverged());

        this->ObtainResults();
    }


    Eigen::VectorX<Scalar> getEigenvalues() const {
        return eigenvalues;
    }

    Eigen::MatrixX<Scalar> getEigenvectors() const {
        return eigenvectors;
    }

protected:
    bool hasConverged() const{
        Scalar diff = (previousVector - currentVector).norm();
        return diff < this->tolerance || currentIterations >= this->maxIterations;
    }

    void performIteration(){
        previousVector = currentVector;
        currentVector = shiftedMatrix * currentVector;
        currentVector.normalize();
    }


    void initialize(){
        currentVector = Eigen::MatrixX<Scalar>::Random(matrix.rows(), 1);
        previousVector = currentVector;
    }

    void ObtainResults(){
        Eigen::Matrix<Scalar, Eigen::Dynamic, 1> vectorizedCurrentVector = currentVector;
        Scalar eigenvalue = (matrix * vectorizedCurrentVector).dot(vectorizedCurrentVector) / vectorizedCurrentVector.squaredNorm();

        this->eigenvalues = Eigen::VectorX<Scalar>::Constant(1, eigenvalue + shift); // 考虑到位移
        this->eigenvectors = vectorizedCurrentVector;
    }

private:
    Eigen::MatrixX<Scalar> matrix;
    Eigen::MatrixX<Scalar> shiftedMatrix; // 位移后的矩阵
    Eigen::MatrixX<Scalar> currentVector;
    Eigen::MatrixX<Scalar> previousVector;
    Eigen::VectorX<Scalar> eigenvalues;
    Eigen::MatrixX<Scalar> eigenvectors;
    int maxIterations;
    int currentIterations;
    Scalar shift; // 位移
    Scalar tolerance;
};

#endif // POWER_METHOD_WITH_SHIFT_H
