#include <gtest/gtest.h>
#include <complex>
#include <algorithms/InversePowerMethodWithShift.h>

/**
 * @class InversePowerMethodWithShiftComplexTest
 * @brief A test fixture for testing the Inverse Power Method with shift for complex matrices.
 *
 * This class provides a set of unit tests for the InversePowerMethodWithShift algorithm specialized
 * for complex numbers, using the Google Test framework. It includes tests for accurate computation
 * of eigenvalues and eigenvectors, as well as exception handling for iteration limits.
 */
class InversePowerMethodWithShiftComplexTest : public ::testing::Test
{
protected:
    InversePowerMethodWithShift<std::complex<double>> solver; ///< An instance of the InversePowerMethodWithShift solver for complex numbers.
    Eigen::MatrixX<std::complex<double>> matrix;             ///< The complex matrix to be used in the tests.
    double tolerance;                                        ///< The tolerance level for the convergence of the algorithm.
    int maxIterations;                                       ///< The maximum number of iterations allowed for the algorithm.
    std::complex<double> shift;                              ///< The shift value used in the Inverse Power Method with shift.

    /**
     * @brief Sets up the test environment before each test is run.
     *
     * Initializes a complex matrix with predefined values, sets the tolerance, maximum number
     * of iterations, and the shift value for the InversePowerMethodWithShift solver.
     */
    void SetUp() override
    {
        matrix.resize(2, 2);
        matrix << std::complex<double>(-2.0 / 7, 0), std::complex<double>(3.0 / 7, -2.0 / 7),
                std::complex<double>(3.0 / 7, 2.0 / 7), std::complex<double>(-3.0 / 7, 0);
        tolerance = 1e-6f;
        maxIterations = 1000;
        shift = -2.0f;
        solver.setShift(shift);
        solver.setMatrix(matrix);
        solver.setTolerance(tolerance);
        solver.setMaxIterations(maxIterations);
    }
};

/**
 * @brief Tests if the InversePowerMethodWithShift correctly solves for the eigenvalues and eigenvectors of a complex matrix.
 *
 * This test verifies that the solver computes the eigenvalues and eigenvectors of a complex matrix accurately
 * within the specified tolerance, taking into account the shift value.
 *
 * For the verification of the eigenvector, we assert if A * v = lambda * v, where A is the matrix, v is the eigenvector
 */
TEST_F(InversePowerMethodWithShiftComplexTest, SolvesCorrectly)
{
    solver.solve();

    auto eigenvalues = solver.getEigenvalues();
    auto eigenvectors = solver.getEigenvectors();

    std::complex<double> firstEigenvalue = std::complex<double>(eigenvalues(0).real(), eigenvalues(0).imag());

    EXPECT_NEAR(firstEigenvalue.real(), -0.877151, 1e-5);
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
 * This test checks if the InversePowerMethodWithShift correctly throws an IterationLimitExceeded exception
 * when the number of iterations exceeds the specified limit for a complex matrix.
 */
TEST_F(InversePowerMethodWithShiftComplexTest, ThrowsIterationLimitExceeded)
{
    this->matrix.resize(2, 2);
    this->matrix << std::complex<double>(0, 2), std::complex<double>(0, -1),
            std::complex<double>(0, -1), std::complex<double>(0, 2);
    this->solver.setMatrix(this->matrix);
    this->solver.setTolerance(this->tolerance);
    this->solver.setMaxIterations(1);

    EXPECT_THROW(this->solver.solve(), IterationLimitExceeded);
}
