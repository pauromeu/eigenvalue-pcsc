#include <gtest/gtest.h>
#include <algorithms/InversePowerMethodWithShift.h>
#include <Eigen/Dense>

/**
 * @class InversePowerMethodWithShiftTest
 * @brief A test fixture for testing the Inverse Power Method with shift algorithm.
 *
 * This class provides a set of unit tests for the InversePowerMethodWithShift algorithm,
 * using the Google Test framework. It tests the algorithm with different types of scalar values
 * and includes tests for correct solution, iteration limit exceeded, and handling of invalid input.
 *
 * @tparam Scalar The type of the scalar values used in the matrix (e.g., double, float).
 */
template <typename Scalar>
class InversePowerMethodWithShiftTest : public ::testing::Test
{
protected:
    InversePowerMethodWithShift<Scalar> solver; ///< An instance of the InversePowerMethodWithShift solver.
    Eigen::MatrixX<Scalar> matrix;             ///< The matrix to be used in the tests.
    Scalar tolerance;                          ///< The tolerance level for the convergence of the algorithm.
    int maxIterations;                         ///< The maximum number of iterations allowed for the algorithm.
    Scalar shift;                              ///< The shift value used in the Inverse Power Method with shift.

    /**
     * @brief Sets up the test environment before each test is run.
     *
     * Initializes the matrix with predefined values, sets the tolerance, maximum number
     * of iterations, and the shift value for the InversePowerMethodWithShift solver.
     */
    void SetUp() override
    {
        matrix.resize(2, 2);
        matrix << 5, 2,
                2, 6;
        tolerance = 1e-6;
        maxIterations = 1000;
        shift = 1.0;

        solver.setShift(shift);
        solver.setMatrix(matrix);
        solver.setTolerance(tolerance);
        solver.setMaxIterations(maxIterations);
    }
};

typedef ::testing::Types<double, float> Implementations; ///< Defines the scalar types to be used in the tests.
TYPED_TEST_SUITE(InversePowerMethodWithShiftTest, Implementations);

/**
 * @brief Tests if the InversePowerMethodWithShift correctly solves for the eigenvalues and eigenvectors.
 *
 * This test verifies that the solver computes the eigenvalues and eigenvectors accurately
 * within the specified tolerance, taking into account the shift value.
 *
 * For the verification of the eigenvector, the cosine similarity between the actual and expected is used.
 */
TYPED_TEST(InversePowerMethodWithShiftTest, SolvesCorrectly)
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
 * This test checks if the InversePowerMethodWithShift correctly throws an IterationLimitExceeded exception
 * when the number of iterations exceeds the specified limit.
 */
TYPED_TEST(InversePowerMethodWithShiftTest, ThrowsIterationLimitExceeded)
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
 * This test ensures that the InversePowerMethodWithShift throws an InvalidInputException when
 * an empty matrix is provided as input.
 */
TYPED_TEST(InversePowerMethodWithShiftTest, ThrowsInvalidInputExceptionForEmptyMatrix)
{
    Eigen::MatrixX<TypeParam> emptyMatrix;

    EXPECT_THROW(this->solver.setMatrix(emptyMatrix), InvalidInputException);
}
