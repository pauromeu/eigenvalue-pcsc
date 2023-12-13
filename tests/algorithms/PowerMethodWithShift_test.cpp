#include <gtest/gtest.h>
#include <algorithms/PowerMethodWithShift.h>
#include <Eigen/Dense>

/**
 * @class PowerMethodWithShiftTest
 * @brief A test fixture for testing the Power Method with shift algorithm.
 *
 * This class provides a set of unit tests for the PowerMethodWithShift algorithm,
 * using the Google Test framework. It tests the algorithm with different scalar types
 * and includes tests for correct solution, iteration limit exceeded, and handling of invalid input.
 *
 * @tparam Scalar The type of the scalar values used in the matrix (e.g., double, float).
 */
template <typename Scalar>
class PowerMethodWithShiftTest : public ::testing::Test
{
protected:
    PowerMethodWithShift<Scalar> solver; ///< An instance of the PowerMethodWithShift solver.
    Eigen::MatrixX<Scalar> matrix; ///< The matrix to be used in the tests.
    Scalar tolerance; ///< The tolerance level for the convergence of the algorithm.
    int maxIterations; ///< The maximum number of iterations allowed for the algorithm.
    Scalar shift; ///< The shift value used in the Power Method with shift.

    /**
     * @brief Sets up the test environment before each test is run.
     *
     * Initializes the matrix with predefined values, sets the tolerance, maximum number
     * of iterations, and the shift value for the PowerMethodWithShift solver.
     */
    void SetUp() override
    {
        matrix.resize(2, 2);
        matrix << 2, 1,
                1, 2;
        tolerance = 1e-6;
        maxIterations = 1000;
        shift = -5.0;

        solver.setShift(shift);
        solver.setMatrix(matrix);
        solver.setTolerance(tolerance);
        solver.setMaxIterations(maxIterations);
    }
};

typedef ::testing::Types<double, float> Implementations; ///< Defines the scalar types to be used in the tests.
TYPED_TEST_SUITE(PowerMethodWithShiftTest, Implementations);

/**
 * @brief Tests if the PowerMethodWithShift correctly solves for the largest eigenvalue and its corresponding eigenvector.
 *
 * This test verifies that the solver computes the largest eigenvalue and its corresponding eigenvector accurately
 * within the specified tolerance, considering the shift value.
 *
 * For the verification of the eigenvector, the cosine similarity between the actual and expected eigenvectors is computed.e
 */
TYPED_TEST(PowerMethodWithShiftTest, SolvesCorrectly)
{
    this->solver.solve();

    Eigen::MatrixX<TypeParam> eigenvectors = this->solver.getEigenvectors();
    Eigen::VectorX<std::complex<double>> eigenvalues = this->solver.getEigenvalues();

    ASSERT_NEAR(eigenvalues(0).real(), 3.0, this->tolerance);

    Eigen::VectorX<TypeParam> actualVector = eigenvectors.col(0);
    Eigen::VectorX<TypeParam> expectedVector(2);
    expectedVector << 1 / sqrt(2), 1 / sqrt(2);

    TypeParam cosSim = actualVector.normalized().dot(expectedVector.normalized());
    ASSERT_NEAR(std::abs(cosSim), 1.0, this->tolerance);
}

/**
 * @brief Tests if the solver throws an exception when the iteration limit is exceeded.
 *
 * This test checks if the PowerMethodWithShift correctly throws an IterationLimitExceeded exception
 * when the number of iterations exceeds the specified limit.
 */
TYPED_TEST(PowerMethodWithShiftTest, ThrowsIterationLimitExceeded)
{
    this->matrix.resize(2, 2);
    this->matrix << 1, 2,
            2, 1;
    this->solver.setMatrix(this->matrix);
    this->solver.setTolerance(this->tolerance);
    this->solver.setMaxIterations(1);

    EXPECT_THROW(this->solver.solve(), IterationLimitExceeded);
}

/**
 * @brief Tests if the solver throws an exception for an empty input matrix.
 *
 * This test ensures that the PowerMethodWithShift throws an InvalidInputException when
 * an empty matrix is provided as input.
 */
TYPED_TEST(PowerMethodWithShiftTest, ThrowsInvalidInputExceptionForEmptyMatrix)
{
    Eigen::MatrixX<TypeParam> emptyMatrix;

    EXPECT_THROW(this->solver.setMatrix(emptyMatrix), InvalidInputException);
}
