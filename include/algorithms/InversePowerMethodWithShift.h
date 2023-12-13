#ifndef INVERSE_POWER_METHOD_WITH_SHIFT_H
#define INVERSE_POWER_METHOD_WITH_SHIFT_H

#include "AbstractPowerMethod.h"

/**
 * @class InversePowerMethodWithShift
 * @brief A class for solving eigenvalue problems using the inverse power method with shift.
 *
 * @tparam Scalar
 * InversePowerMethodWithShift is a class that solves eigenvalue problems using the inverse power method with shift.
 */
template <typename Scalar>
class InversePowerMethodWithShift : public AbstractPowerMethod<Scalar>
{
public:
    using AbstractPowerMethod<Scalar>::AbstractPowerMethod;

    /**
     * @brief set the matrix.
     * This method sets the matrix to be used in the inverse power method, and computes its inverse, with shift.
     * @param mat The matrix to be set.
     * @throws InvalidInputException if the matrix is empty.
     */
    void setMatrix(const Eigen::MatrixX<Scalar>& mat) {
        if (mat.rows() == 0 || mat.cols() == 0) {
            throw InvalidInputException("Matrix cannot be empty");
        }
        this->matrix = mat;
        shiftedMatrix = this->matrix - shift * Eigen::MatrixX<Scalar>::Identity(this->matrix.rows(), this->matrix.cols());
        matrixInverse = shiftedMatrix.inverse();
    }

    /**
     * @brief set the shift.
     * This method sets the shift to be used in the inverse power method.
     * @param sh The shift to be set.
     */
    void setShift(Scalar sh) {
        shift = sh;
    }


protected:
    /**
     * @brief perform an iteration.
     * This method performs an iteration of the inverse power method with shift.
     * @return void
     */
    void performIteration() {
        this->previousVector = this->currentVector;
        this->currentVector = matrixInverse * this->currentVector;
        this->currentVector.normalize();
    }

    /**
     * @brief obtain the results.
     * This method obtains the results of the inverse power method with shift.
     * It retrieves the eigenvector from iteration.
     * Then, it computes the eigenvalue using the Rayleigh quotient.
     * Finally, it stores the results in the corresponding attributes.
     * @return void
     */
    void obtainResults() {
        Eigen::Matrix<Scalar, Eigen::Dynamic, 1> vectorizedCurrentVector = this->currentVector;
        Scalar eigenvalue = (matrixInverse * vectorizedCurrentVector).dot(vectorizedCurrentVector) / vectorizedCurrentVector.squaredNorm();
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
