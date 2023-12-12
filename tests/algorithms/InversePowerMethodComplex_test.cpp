#include <gtest/gtest.h>
#include <complex>
#include <algorithms/InversePowerMethod.h>

class InversePowerMethodComplexTest : public ::testing::Test
{
protected:
    InversePowerMethod<std::complex<double>> solver;
    Eigen::MatrixX<std::complex<double>> matrix;
    double tolerance;
    int maxIterations;

    void SetUp() override
    {
        matrix.resize(2, 2);
        matrix << std::complex<double>(-2.0 / 7, 0), std::complex<double>(3.0 / 7, -2.0 / 7),
            std::complex<double>(3.0 / 7, 2.0 / 7), std::complex<double>(-3.0 / 7, 0);
        tolerance = 1e-6f;
        maxIterations = 1000;

        solver.setMatrix(matrix);
        solver.setTolerance(tolerance);
        solver.setMaxIterations(maxIterations);
    }
};

TEST_F(InversePowerMethodComplexTest, SolvesCorrectly)
{
    solver.solve();

    auto eigenvalues = solver.getEigenvalues();
    auto eigenvectors = solver.getEigenvectors();

    std::complex<double> firstEigenvalue = std::complex<double>(eigenvalues(0).real(), eigenvalues(0).imag());

    EXPECT_NEAR(firstEigenvalue.real(), 0.162865, 1e-5);
    EXPECT_NEAR(firstEigenvalue.imag(), 0, tolerance);

    Eigen::VectorX<std::complex<double>> actualVector = eigenvectors.col(0);
    Eigen::VectorX<std::complex<double>> expectedVector(2);

    Eigen::VectorX<std::complex<double>> result1 = matrix * actualVector;

    Eigen::VectorX<std::complex<double>> result2 = firstEigenvalue * actualVector;

    double tolerance = 1e-5;
    for (int i = 0; i < result1.size(); ++i)
    {
        EXPECT_NEAR(result1(i).real(), result2(i).real(), tolerance);
        EXPECT_NEAR(result1(i).imag(), result2(i).imag(), tolerance);
    }
}

TEST_F(InversePowerMethodComplexTest, ThrowsIterationLimitExceeded)
{
    this->matrix.resize(2, 2);
    this->matrix << std::complex<double>(0, 2), std::complex<double>(0, -1),
        std::complex<double>(0, -1), std::complex<double>(0, 2);
    this->solver.setMatrix(this->matrix);
    this->solver.setTolerance(this->tolerance);
    this->solver.setMaxIterations(1);

    EXPECT_THROW(this->solver.solve(), IterationLimitExceeded);
}
