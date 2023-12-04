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

    // 纯虚函数定义，确保子类实现这些方法
    virtual void solve() override = 0;

protected:
    // 特定于power method的辅助函数
    virtual bool hasConverged() const override = 0;
    virtual void performIteration() override = 0;
    virtual void initialize() override = 0;

    // power method的特定数据
    Eigen::MatrixX<Scalar> currentVector;
};

#endif // ABSTRACT_POWER_METHOD_H

