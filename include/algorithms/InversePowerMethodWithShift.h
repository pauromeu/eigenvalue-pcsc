#ifndef INVERSE_POWER_METHOD_WITH_SHIFT_H
#define INVERSE_POWER_METHOD_WITH_SHIFT_H

#include "AbstractPowerMethod.h"

template <typename Scalar>
class InversePowerMethodWithShift : public AbstractPowerMethod<Scalar>
{
public:
    using AbstractPowerMethod<Scalar>::AbstractPowerMethod;

    void setMatrix(const Eigen::MatrixX<Scalar>& mat) {
        if (mat.rows() == 0 || mat.cols() == 0) {
            throw InvalidInputException("Matrix cannot be empty");
        }
        this->matrix = mat;
        shiftedMatrix = this->matrix - shift * Eigen::MatrixX<Scalar>::Identity(this->matrix.rows(), this->matrix.cols());
        matrixInverse = shiftedMatrix.inverse();
    }


    void setShift(Scalar sh) {
        shift = sh;
    }


protected:
    void performIteration() {
        this->previousVector = this->currentVector;
        this->currentVector = matrixInverse * this->currentVector;
        this->currentVector.normalize();
    }

    void obtainResults() {
        Eigen::Matrix<Scalar, Eigen::Dynamic, 1> vectorizedCurrentVector = this->currentVector;
        Scalar eigenvalue = (matrixInverse * vectorizedCurrentVector).dot(vectorizedCurrentVector) / vectorizedCurrentVector.squaredNorm();

//        this->eigenvalues = Eigen::VectorX<Scalar>::Constant(1, 1.0 / eigenvalue + shift);
//        this->eigenvectors = vectorizedCurrentVector;
        if constexpr (std::is_same<Scalar, std::complex<float>>::value || std::is_same<Scalar, std::complex<double>>::value) {
            Scalar one = Scalar(1.0, 0.0);
            this->eigenvalues = Eigen::VectorX<Scalar>::Constant(1, one / eigenvalue + shift);
        } else {
            this->eigenvalues = Eigen::VectorX<Scalar>::Constant(1, 1 / eigenvalue + shift);
        }
        this->eigenvectors = vectorizedCurrentVector;
    }


private:
    Eigen::MatrixX<Scalar> matrixInverse;
    Eigen::MatrixX<Scalar> shiftedMatrix;
    Scalar shift;
};

#endif // INVERSE_POWER_METHOD_WITH_SHIFT_H
