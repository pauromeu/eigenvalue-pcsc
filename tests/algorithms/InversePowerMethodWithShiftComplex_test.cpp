#include <gtest/gtest.h>
#include <complex>
#include <algorithms/InversePowerMethodWithShift.h>

class InversePowerMethodWithShiftComplexTest : public ::testing::Test {
protected:
    InversePowerMethodWithShift<std::complex<float>> solver;
    Eigen::MatrixX<std::complex<float>> matrix;
    float tolerance;
    int maxIterations;
    std::complex<float> shift = std::complex<float>(2.0f, 0.0f);

    void SetUp() override {
        matrix.resize(2, 2);
        matrix << std::complex<float>(-2.0/7, 0), std::complex<float>(3.0/7, -2.0/7),
                std::complex<float>(3.0/7, 2.0/7), std::complex<float>(-3.0/7, 0);
        tolerance = 1e-6f;
        maxIterations = 1000;
        shift = -2.0f;
        solver.setShift(shift);
        solver.setMatrix(matrix);
        solver.setTolerance(tolerance);
        solver.setMaxIterations(maxIterations);
    }
};

TEST_F(InversePowerMethodWithShiftComplexTest, SolvesCorrectly) {
    solver.solve();

    auto eigenvalues = solver.getEigenvalues();
    auto eigenvectors = solver.getEigenvectors();

    std::complex<float> firstEigenvalue = std::complex<float>(eigenvalues(0).real(), eigenvalues(0).imag());

    EXPECT_NEAR(firstEigenvalue.real(), -0.877151, 1e-5);
    EXPECT_NEAR(firstEigenvalue.imag(), 0, tolerance);

    Eigen::VectorX<std::complex<float>> actualVector = eigenvectors.col(0);
    Eigen::VectorX<std::complex<float>> expectedVector(2);

    Eigen::VectorX<std::complex<float>> result1 = matrix * actualVector;

    Eigen::VectorX<std::complex<float>> result2 = firstEigenvalue * actualVector;

    float tolerance = 1e-5;
    for (int i = 0; i < result1.size(); ++i) {
        EXPECT_NEAR(result1(i).real(), result2(i).real(), tolerance);
        EXPECT_NEAR(result1(i).imag(), result2(i).imag(), tolerance);
    }
}

TEST_F(InversePowerMethodWithShiftComplexTest, ThrowsIterationLimitExceeded) {
    this->matrix.resize(2, 2);
    this-> matrix << std::complex<float>(0, 2), std::complex<float>(0, -1),
            std::complex<float>(0, -1), std::complex<float>(0, 2);
    this->solver.setMatrix(this->matrix);
    this->solver.setTolerance(this->tolerance);
    this->solver.setMaxIterations(1);

    EXPECT_THROW(this->solver.solve(), IterationLimitExceeded);
}
