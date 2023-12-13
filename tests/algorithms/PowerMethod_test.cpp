#include <gtest/gtest.h>
#include <algorithms/PowerMethod.h>
#include <Eigen/Dense>

/**
 * @class PowerMethodTest
 * @brief A test fixture for testing the Power Method algorithm.
 *
 * This class provides a set of unit tests for the PowerMethod algorithm,
 * using the Google Test framework. It tests the algorithm with different scalar types
 * and includes tests for correct solution, iteration limit exceeded, and handling of invalid input.
 *
 * @tparam Scalar The type of the scalar values used in the matrix (e.g., double, float).
 */
template <typename Scalar>
class PowerMethodTest : public ::testing::Test
{
protected:
    PowerMethod<Scalar> solver; ///< An instance of the PowerMethod solver.
    Eigen::MatrixX<Scalar> matrix; ///< The matrix to be used in the tests.
    Scalar tolerance; ///< The tolerance level for the convergence of the algorithm.
    int maxIterations; ///< The maximum number of iterations allowed for the algorithm.

    /**
     * @brief Sets up the test environment before each test is run.
     *
     * Initializes the matrix with predefined values, sets the tolerance,
     * and the maximum number of iterations for the PowerMethod solver.
     */
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

typedef ::testing::Types<double, float> Implementations; ///< Defines the scalar types to be used in the tests.
TYPED_TEST_SUITE(PowerMethodTest, Implementations);

/**
 * @brief Tests if the PowerMethod correctly solves for the largest eigenvalue and its corresponding eigenvector.
 *
 * This test verifies that the solver computes the largest eigenvalue and its corresponding eigenvector accurately
 * within the specified tolerance.
 *
 * For the verification of the eigenvector, the cosine similarity between the actual and expected eigenvectors is used.n
 */
TYPED_TEST(PowerMethodTest, SolvesCorrectly)
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
 * This test checks if the PowerMethod correctly throws an IterationLimitExceeded exception
 * when the number of iterations exceeds the specified limit.
 */
TYPED_TEST(PowerMethodTest, ThrowsIterationLimitExceeded)
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
 * This test ensures that the PowerMethod throws an InvalidInputException when
 * an empty matrix is provided as input.
 */
TYPED_TEST(PowerMethodTest, ThrowsInvalidInputExceptionForEmptyMatrix)
{
    Eigen::MatrixX<TypeParam> emptyMatrix;

    EXPECT_THROW(this->solver.setMatrix(emptyMatrix), InvalidInputException);
}
