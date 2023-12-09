#ifndef POWER_METHOD_WITH_SHIFT_H
#define POWER_METHOD_WITH_SHIFT_H

#include "AbstractPowerMethod.h"

template <typename Scalar>
class PowerMethodWithShift : public AbstractPowerMethod<Scalar>
{
public:
    using AbstractPowerMethod<Scalar>::AbstractPowerMethod;

    void setMatrix(const Eigen::MatrixX<Scalar>& mat) {
        if (mat.rows() == 0 || mat.cols() == 0) {
            throw InvalidInputException("Matrix cannot be empty");
        }
        this->matrix = mat;
        shiftedMatrix = this->matrix - shift * Eigen::MatrixX<Scalar>::Identity(this->matrix.rows(), this->matrix.cols());
    }

    void setShift(Scalar sh) {
        shift = sh;
    }

protected:
    void performIteration(){
        this->previousVector = this->currentVector;
        this->currentVector = shiftedMatrix * this->currentVector;
        this->currentVector.normalize();
    }

    void obtainResults() {
        Eigen::Matrix<Scalar, Eigen::Dynamic, 1> vectorizedCurrentVector = this->currentVector;
        Scalar eigenvalue = (shiftedMatrix * vectorizedCurrentVector).dot(vectorizedCurrentVector) / vectorizedCurrentVector.squaredNorm();

        this->eigenvalues = Eigen::VectorX<Scalar>::Constant(1, eigenvalue + shift);
        this->eigenvectors = vectorizedCurrentVector;
    }

private:
    Eigen::MatrixX<Scalar> shiftedMatrix;
    Scalar shift;
};

#endif // POWER_METHOD_WITH_SHIFT_H
