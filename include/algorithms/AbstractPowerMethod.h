#ifndef ABSTRACT_POWER_METHOD_H
#define ABSTRACT_POWER_METHOD_H

#include <Eigen/Dense>
#include <complex>
#include "EigenvalueSolver.h"

template <typename Scalar>
class AbstractPowerMethod : public EigenvalueSolver<Scalar>
{
public:
    using EigenvalueSolver<Scalar>::EigenvalueSolver;

    virtual void solve() override = 0;

protected:
    virtual bool hasConverged() const override = 0;
    virtual void performIteration() override = 0;
    virtual void initialize() override = 0;

    Eigen::MatrixX<Scalar> currentVector;
};

#endif // ABSTRACT_POWER_METHOD_H

