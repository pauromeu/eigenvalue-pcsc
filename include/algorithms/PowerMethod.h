//
// Created by Tachi on 2023/12/2.
//

#ifndef POWER_METHOD_H
#define POWER_METHOD_H

#include "AbstractPowerMethod.h"
#include <Eigen/Dense>


template <typename Scalar>
class PowerMethod : public AbstractPowerMethod<Scalar>
{
public:
    PowerMethod(int maxIterations = 100, Scalar tolerance = static_cast<Scalar>(1e-6));
    virtual ~PowerMethod(); // 析构函数

    void setMatrix(const Eigen::MatrixX<Scalar> &matrix) override;
    void solve() override;
    int getMaxIterationsPublic() const {
        return this->getMaxIterations(); // 调用受保护的成员函数
    }
    int getTolerancePublic() const {
        return this->getTolerance(); // 调用受保护的成员函数
    }

private:
protected:
    void initialize() override;
    void performIteration() override;
    bool hasConverged() const override;

};

#endif // POWER_METHOD_H
