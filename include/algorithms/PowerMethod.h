#ifndef POWER_METHOD_H
#define POWER_METHOD_H

#include "AbstractPowerMethod.h"

/**
 * @class PowerMethod
 * @brief A class for solving eigenvalue problems using the power method.
 *
 * @tparam Scalar
 * PowerMethod is a class that solves eigenvalue problems using the power method.
 */
template <typename Scalar>
class PowerMethod : public AbstractPowerMethod<Scalar>
{
public:
    using AbstractPowerMethod<Scalar>::AbstractPowerMethod;
    /**
     * @brief set the matrix.
     * This method sets the matrix to be used in the power method.
     * @param mat The matrix to be set.
     * @throws InvalidInputException if the matrix is empty.
     */
    void setMatrix(const Eigen::MatrixX<Scalar>& mat) {
        if (mat.rows() == 0 || mat.cols() == 0) {
            throw InvalidInputException("Matrix cannot be empty");
        }
        this->matrix = mat;
    }

protected:
    /**
     * @brief perform an iteration.
     * This method performs an iteration of the power method.
     * @return void
     */
    void performIteration() {
        this->previousVector = this->currentVector;
        this->currentVector = this->matrix * this->currentVector;
        this->currentVector.normalize();
    }
    /**
     * @brief obtain the results.
     * This method obtains the results of the power method.
     * It retrieves the eigenvector from iteration.
     * Then, it computes the eigenvalue using the Rayleigh quotient.
     * Finally, it stores the results in the corresponding attributes.
     * @return void
     */
    void obtainResults() {
        Eigen::Matrix<Scalar, Eigen::Dynamic, 1> vectorizedCurrentVector = this->currentVector;
        Scalar eigenvalue = (this->matrix * vectorizedCurrentVector).dot(vectorizedCurrentVector) / vectorizedCurrentVector.squaredNorm();

        this->eigenvalues = Eigen::VectorX<Scalar>::Constant(1, eigenvalue);
        this->eigenvectors = vectorizedCurrentVector;
    }
};

#endif // POWER_METHOD_H
