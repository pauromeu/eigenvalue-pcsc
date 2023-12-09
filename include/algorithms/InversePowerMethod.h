#ifndef INVERSE_POWER_METHOD_H
#define INVERSE_POWER_METHOD_H

#include "AbstractPowerMethod.h"

template <typename Scalar>
class InversePowerMethod : public AbstractPowerMethod<Scalar>
{
public:
    using AbstractPowerMethod<Scalar>::AbstractPowerMethod;

    void setMatrix(const Eigen::MatrixX<Scalar>& mat){
        this->matrix = mat;
        matrixInverse = this->matrix.inverse();
    }


protected:
    Eigen::MatrixX<Scalar> matrixInverse;

    void performIteration(){
        this->previousVector = this->currentVector;
        this->currentVector = matrixInverse * this->currentVector;
        this->currentVector.normalize();
    }

    void obtainResults(){
        Eigen::Matrix<Scalar, Eigen::Dynamic, 1> vectorizedCurrentVector = this->currentVector;
        Scalar eigenvalue = (matrixInverse * vectorizedCurrentVector).dot(vectorizedCurrentVector) / vectorizedCurrentVector.squaredNorm();

        this->eigenvalues = Eigen::VectorX<Scalar>::Constant(1, 1 / eigenvalue);
        this->eigenvectors = vectorizedCurrentVector;
    }
};

#endif // INVERSE_POWER_METHOD_H
