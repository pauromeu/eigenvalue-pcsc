#include <algorithms/EigenvalueSolver.h>

#ifndef QRMETHOD_H
#define QRMETHOD_H

#include "EigenvalueSolver.h"
#include <Eigen/Dense>

template <typename Scalar>
class QRMethod : public EigenvalueSolver<Scalar>
{
public:
    QRMethod(int maxIterations = 100, Scalar tol = 1e-6);

    void setMatrix(const Eigen::MatrixX<Scalar> &matrix) override;
    void solve() override;

protected:
    bool hasConverged() const override;
    void performIteration() override;
    void initialize() override;

private:
    Eigen::MatrixX<Scalar> matrix;
    int currentIteration;
    Scalar tolerance;
};

#endif // QRMETHOD_H
