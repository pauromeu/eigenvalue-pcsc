//
// Created by Tachi on 2023/12/2.
//
/*
 * Description:
 * @brief AbstractPowerMethod is an abstract class for power method.
 * */
#ifndef ABSTRACT_POWER_METHOD_H
#define ABSTRACT_POWER_METHOD_H


#include "EigenvalueSolver.h"

template <typename Scalar>
class AbstractPowerMethod : public EigenvalueSolver<Scalar>
{
public:
    virtual ~AbstractPowerMethod() {}

protected:
    void setMatrix(const Eigen::MatrixX<Scalar>& mat) { this->matrix = mat; }
    void setMaxIterations(int maxIter) { this->maxIterations = maxIter; }
    int getMaxIterations() const { return this->maxIterations; }
    void setTolerance(Scalar tol) { this->tolerance = tol; }
    Scalar getTolerance() const { return this->tolerance; }

    const Eigen::MatrixX<Scalar>& getMatrix() const { return this->matrix; }
    void setEigenvalues(const Eigen::VectorX<Scalar>& vals) { this->eigenvalues = vals; }
    void setEigenvectors(const Eigen::MatrixX<Scalar>& vecs) { this->eigenvectors = vecs; }

    virtual void solve() override = 0;

private:
    Eigen::MatrixX<Scalar> matrix;
    Eigen::VectorX<Scalar> eigenvalues;
    Eigen::MatrixX<Scalar> eigenvectors;
    int maxIterations;
    Scalar tolerance;
};


#endif // ABSTRACT_POWER_METHOD_H

