#ifndef POWER_METHOD_H
#define POWER_METHOD_H

#include "AbstractPowerMethod.h"

template <typename Scalar>
class PowerMethod : public AbstractPowerMethod<Scalar>
{
public:
    using AbstractPowerMethod<Scalar>::AbstractPowerMethod;
    void setMatrix(const Eigen::MatrixX<Scalar>& mat) {
        this->matrix = mat;
    }

protected:
    void performIteration() {
        this->previousVector = this->currentVector;
        this->currentVector = this->matrix * this->currentVector;
        this->currentVector.normalize();
    }

    void obtainResults() {
        Eigen::Matrix<Scalar, Eigen::Dynamic, 1> vectorizedCurrentVector = this->currentVector;
        Scalar eigenvalue = (this->matrix * vectorizedCurrentVector).dot(vectorizedCurrentVector) / vectorizedCurrentVector.squaredNorm();

        this->eigenvalues = Eigen::VectorX<Scalar>::Constant(1, eigenvalue);
        this->eigenvectors = vectorizedCurrentVector;
    }
};

#endif // POWER_METHOD_H
