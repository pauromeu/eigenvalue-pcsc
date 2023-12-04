#include <gtest/gtest.h>
#include <algorithms/InversePowerMethodWithShift.h>
#include <Eigen/Dense>

template <typename Scalar>
class InversePowerMethodWithShiftTest : public ::testing::Test {
protected:
    InversePowerMethodWithShift<Scalar> solver;
    Eigen::MatrixX<Scalar> matrix;
    Scalar tolerance;
    int maxIterations;
    Scalar shift;

    void SetUp() override {
        matrix.resize(2, 2);
        matrix << 2, 1,
                1, 2;
        tolerance = 1e-6;
        maxIterations = 1000;
        shift = 5.0;

        solver.setShift(shift);
        solver.setMatrix(matrix);
        solver.setTolerance(tolerance);
        solver.setMaxIterations(maxIterations);
    }
};

typedef ::testing::Types<double, float> Implementations;
TYPED_TEST_SUITE(InversePowerMethodWithShiftTest, Implementations);

TYPED_TEST(InversePowerMethodWithShiftTest, SolvesCorrectly) {
    this->solver.solve();

    Eigen::MatrixX<TypeParam> eigenvectors = this->solver.getEigenvectors();
    Eigen::VectorX<TypeParam> eigenvalues = this->solver.getEigenvalues();

    ASSERT_NEAR(eigenvalues(0), 3.0, this->tolerance);

    Eigen::VectorX<TypeParam> actualVector = eigenvectors.col(0);
    Eigen::VectorX<TypeParam> expectedVector(2);
    expectedVector << 1/sqrt(2), 1/sqrt(2);

    TypeParam cosSim = actualVector.normalized().dot(expectedVector.normalized());
    ASSERT_NEAR(std::abs(cosSim), 1.0, this->tolerance);
}

