#include <gtest/gtest.h>
#include "algorithms/QRMethod.h"
#include <Eigen/Dense>

template <typename Scalar>
class QRMethodTest : public ::testing::Test
{
protected:
    QRMethod<Scalar> solver;
    Eigen::MatrixX<Scalar> matrix;
    Scalar tolerance;
    int maxIterations;

    void SetUp() override
    {
        matrix.resize(2, 2);
        matrix << 2, 1,
            1, 2;
        tolerance = 1e-6;
        maxIterations = 1000;

        solver.setMatrix(matrix);
        solver.setTolerance(tolerance);
        solver.setMaxIterations(maxIterations);
    }
};

typedef ::testing::Types<double, float> Implementations;
TYPED_TEST_SUITE(QRMethodTest, Implementations);

TYPED_TEST(QRMethodTest, SolvesCorrectly)
{
    this->solver.solve();

    auto eigenvalues = this->solver.getEigenvalues();

    ASSERT_NEAR(std::abs(eigenvalues(0)), 3.0, this->tolerance);
}
