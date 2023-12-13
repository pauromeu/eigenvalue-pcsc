#ifndef POWER_METHOD_WITH_SHIFT_H
#define POWER_METHOD_WITH_SHIFT_H

#include "AbstractPowerMethod.h"

template <typename Scalar>
/**
 * @class PowerMethodWithShift
 * @brief A class for solving eigenvalue problems using the power method with shift.
 *
 * PowerMethodWithShift is a class that solves eigenvalue problems using the power method with shift.
 * @tparam Scalar
 */
class PowerMethodWithShift : public AbstractPowerMethod<Scalar>
{
public:
    using AbstractPowerMethod<Scalar>::AbstractPowerMethod;

    /**
     * @brief set the matrix.
     * This method sets the matrix to be used in the power method, with shift.
     * @param mat The matrix to be set.
     * @throws InvalidInputException if the matrix is empty.
     */
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
    /**
     * @brief perform an iteration.
     * This method performs an iteration of the power method with shift.
     * @return void
     */
    void performIteration(){
        this->previousVector = this->currentVector;
        this->currentVector = shiftedMatrix * this->currentVector;
        this->currentVector.normalize();
    }
    /**
     * @brief obtain the results.
     * This method obtains the results of the power method with shift.
     * It retrieves the eigenvector from iteration.
     * Then, it computes the eigenvalue using the Rayleigh quotient.
     * Finally, it stores the results in the corresponding attributes.
     * @return void
     */
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
