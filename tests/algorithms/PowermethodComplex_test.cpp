#include <gtest/gtest.h>
#include <complex>
#include <algorithms/PowerMethod.h>

/**
 * @class PowerMethodComplexTest
 * @brief A test fixture for testing the Power Method algorithm with complex numbers.
 *
 * This class provides a set of unit tests for the PowerMethod algorithm specialized
 * for complex numbers, using the Google Test framework. It tests the algorithm's capability
 * to compute the largest eigenvalue and its corresponding eigenvector for complex matrices,
 * and also checks for proper exception handling.
 */
class PowerMethodComplexTest : public ::testing::Test
{
protected:
    PowerMethod<std::complex<double>> solver; ///< An instance of the PowerMethod solver for complex numbers.
    Eigen::MatrixX<std::complex<double>> matrix; ///< The complex matrix to be used in the tests.
    double tolerance; ///< The tolerance level for the convergence of the algorithm.
    int maxIterations; ///< The maximum number of iterations allowed for the algorithm.

    /**
     * @brief Sets up the test environment before each test is run.
     *
     * Initializes a complex matrix with predefined values, sets the tolerance,
     * and the maximum number of iterations for the PowerMethod solver.
     */
    void SetUp() override
    {
        matrix.resize(2, 2);
        matrix << std::complex<double>(3, 0), std::complex<double>(3, -2),
                std::complex<double>(3, 2), std::complex<double>(2, 0);
        tolerance = 1e-6f;
        maxIterations = 1000;

        solver.setMatrix(matrix);
        solver.setTolerance(tolerance);
        solver.setMaxIterations(maxIterations);
    }
};

/**
 * @brief Tests if the PowerMethod correctly solves for the largest eigenvalue and its corresponding eigenvector in a complex matrix.
 *
 * This test verifies that the solver computes the largest eigenvalue and its corresponding eigenvector of a complex matrix accurately
 * within the specified tolerance.
 *
 * For the verification of the eigenvector, the test multiplies the matrix by the eigenvector and compares the result with the eigenvalue multiplied by the eigenvector.
 */
TEST_F(PowerMethodComplexTest, SolvesCorrectly)
{
    solver.solve();

    auto eigenvalues = solver.getEigenvalues();
    auto eigenvectors = solver.getEigenvectors();

    std::complex<double> firstEigenvalue = std::complex<double>(eigenvalues(0).real(), eigenvalues(0).imag());

    EXPECT_NEAR(firstEigenvalue.real(), 6.14005, 1e-5);
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

/**
 * @brief Tests if the solver throws an exception when the iteration limit is exceeded for a complex matrix.
 *
 * This test checks if the PowerMethod correctly throws an IterationLimitExceeded exception
 * when the number of iterations exceeds the specified limit for a complex matrix.
 */
TEST_F(PowerMethodComplexTest, ThrowsIterationLimitExceeded)
{
    this->matrix.resize(2, 2);
    this->matrix << std::complex<double>(0, 2), std::complex<double>(0, -1),
            std::complex<double>(0, -1), std::complex<double>(0, 2);
    this->solver.setMatrix(this->matrix);
    this->solver.setTolerance(this->tolerance);
    this->solver.setMaxIterations(1);

    EXPECT_THROW(this->solver.solve(), IterationLimitExceeded);
}
