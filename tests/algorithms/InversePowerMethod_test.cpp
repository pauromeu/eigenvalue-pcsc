#include <gtest/gtest.h>
#include <algorithms/InversePowerMethod.h>
#include <Eigen/Dense>

/**
 * @class InversePowerMethodTest
 * @brief A test fixture for testing the Inverse Power Method algorithm.
 *
 * This class provides a set of unit tests for the InversePowerMethod algorithm, using
 * the Google Test framework. It tests the algorithm with different types of scalar values.
 *
 * @tparam Scalar The type of the scalar values used in the matrix (e.g., double, float).
 */
template <typename Scalar>
class InversePowerMethodTest : public ::testing::Test
{
protected:
    InversePowerMethod<Scalar> solver; ///< An instance of the InversePowerMethod solver.
    Eigen::MatrixX<Scalar> matrix;     ///< The matrix to be used in the tests.
    Scalar tolerance;                  ///< The tolerance level for the convergence of the algorithm.
    int maxIterations;                 ///< The maximum number of iterations allowed for the algorithm.

    /**
     * @brief Sets up the test environment before each test is run.
     *
     * Initializes the matrix with predefined values, sets the tolerance, and the maximum number
     * of iterations for the InversePowerMethod solver.
     */
    void SetUp() override
    {
        matrix.resize(2, 2);
        matrix << 5, 2,
                2, 6;
        tolerance = 1e-6;
        maxIterations = 1000;

        solver.setMatrix(matrix);
        solver.setTolerance(tolerance);
        solver.setMaxIterations(maxIterations);
    }
};

typedef ::testing::Types<double, float> Implementations; ///< Defines the scalar types to be used in the tests.
TYPED_TEST_SUITE(InversePowerMethodTest, Implementations);

/**
 * @brief Tests if the InversePowerMethod correctly solves for the eigenvalues and eigenvectors.
 *
 * This test verifies that the solver computes the eigenvalues and eigenvectors accurately
 * within the specified tolerance.
 * For the verification for the eigenvector, the cosine similarity between the actual and expected is used.
 */
TYPED_TEST(InversePowerMethodTest, SolvesCorrectly)
{
    this->solver.solve();

    Eigen::MatrixX<TypeParam> eigenvectors = this->solver.getEigenvectors();
    Eigen::VectorX<std::complex<double>> eigenvalues = this->solver.getEigenvalues();

    ASSERT_NEAR(eigenvalues(0).real(), (11 - sqrt(17)) / 2, this->tolerance);

    Eigen::VectorX<TypeParam> actualVector = eigenvectors.col(0);
    Eigen::VectorX<TypeParam> expectedVector(2);
    expectedVector << (-1 - sqrt(17)) / 4, 1;

    TypeParam cosSim = actualVector.normalized().dot(expectedVector.normalized());
    ASSERT_NEAR(std::abs(cosSim), 1.0, this->tolerance);
}

/**
 * @brief Tests if the solver throws an exception when the iteration limit is exceeded.
 *
 * This test checks if the InversePowerMethod correctly throws an IterationLimitExceeded exception
 * when the number of iterations exceeds the specified limit.
 */
TYPED_TEST(InversePowerMethodTest, ThrowsIterationLimitExceeded)
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
 * This test ensures that the InversePowerMethod throws an InvalidInputException when
 * an empty matrix is provided as input.
 */
TYPED_TEST(InversePowerMethodTest, ThrowsInvalidInputExceptionForEmptyMatrix)
{
    Eigen::MatrixX<TypeParam> emptyMatrix;

    EXPECT_THROW(this->solver.setMatrix(emptyMatrix), InvalidInputException);
}
