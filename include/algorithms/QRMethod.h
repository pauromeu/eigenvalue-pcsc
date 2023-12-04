#include <algorithms/EigenvalueSolver.h>

#ifndef QRMETHOD_H
#define QRMETHOD_H

#include "EigenvalueSolver.h"
#include <Eigen/Dense>

template <typename Scalar>
class QRMethod : public EigenvalueSolver<Scalar>
{
public:
    QRMethod(int maxIterations = 10000000, Scalar tol = 1e-6) : currentIteration(0), maxIterations(maxIterations), tolerance(tol) {}

    void setMatrix(const Eigen::MatrixX<Scalar> &matrix) override;
    void setMaxIterations(int maxIter) override;
    void setTolerance(Scalar tol) override;

    void solve() override
    {
        initialize();
        while (!hasConverged())
        {
            performIteration();
        }
        obtainResults();
    }

protected:
    void initialize() override;
    bool hasConverged() const override;
    void performIteration() override;
    void obtainResults() override;

private:
    Eigen::MatrixX<Scalar> matrix;
    int currentIteration;
    int maxIterations;
    Scalar tolerance;
};

template class QRMethod<double>;
template class QRMethod<float>;
template class QRMethod<std::complex<float>>;
template class QRMethod<std::complex<double>>;

#endif // QRMETHOD_H
