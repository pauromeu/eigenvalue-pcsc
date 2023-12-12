#include <gtest/gtest.h>
#include "algorithms/QRMethod.h"
#include <Eigen/Dense>

template <typename Scalar>
class QRMethodComplexTest : public ::testing::Test
{
protected:
    QRMethod<Scalar> solver;
    Eigen::MatrixX<Scalar> matrix;
    float tolerance;
    int maxIterations;

    void SetUp() override
    {
        matrix.resize(2, 2);
        matrix << std::complex<float>(3, 0), std::complex<float>(3, -2),
            std::complex<float>(3, 2), std::complex<float>(2, 0);
        tolerance = 1e-6;
        maxIterations = 1000;

        solver.setMatrix(matrix);
        solver.setTolerance(tolerance);
        solver.setMaxIterations(maxIterations);
    }
};

typedef ::testing::Types<std::complex<float>, std::complex<float>> Implementations;
TYPED_TEST_SUITE(QRMethodComplexTest, Implementations);

TYPED_TEST(QRMethodComplexTest, SolvesCorrectly)
{
    this->solver.solve();

    auto eigenvalues = this->solver.getEigenvalues();

    ASSERT_NEAR(std::abs(eigenvalues(0).real()), 6.1400556564331055, this->tolerance);
    ASSERT_NEAR(std::abs(eigenvalues(0).imag()), 0.0, this->tolerance);
}
