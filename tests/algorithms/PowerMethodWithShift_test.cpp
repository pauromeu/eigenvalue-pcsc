#include <gtest/gtest.h>
#include <algorithms/PowerMethodWithShift.h>
#include <Eigen/Dense>

// PowerMethodWithShift 类的测试套件
template <typename Scalar>
class PowerMethodWithShiftTest : public ::testing::Test {
protected:
    PowerMethodWithShift<Scalar> solver;
    Eigen::MatrixX<Scalar> matrix;
    Scalar tolerance;
    int maxIterations;
    Scalar shift;

    void SetUp() override {
        // 设置测试中使用的矩阵、公差、最大迭代次数和位移
        matrix.resize(2, 2);
        matrix << 2, 1,
                1, 2;
        tolerance = 1e-6;
        maxIterations = 1000;
        shift = 2.0;  // 选择一个适当的位移量

        solver.setMatrix(matrix);
        solver.setTolerance(tolerance);
        solver.setMaxIterations(maxIterations);
        solver.setShift(shift);
    }
};

// 使用 double 和 float 类型实例化测试
typedef ::testing::Types<double, float> Implementations;
TYPED_TEST_SUITE(PowerMethodWithShiftTest, Implementations);

TYPED_TEST(PowerMethodWithShiftTest, SolvesCorrectly) {
    this->solver.solve();

    // 获取结果
    Eigen::MatrixX<TypeParam> eigenvectors = this->solver.getEigenvectors();
    Eigen::VectorX<TypeParam> eigenvalues = this->solver.getEigenvalues();

    // 验证特征值（假设已知的正确特征值和位移量的关系）
    ASSERT_NEAR(eigenvalues(0), 5.0, this->tolerance);

    // 验证特征向量
    // 注意：特征向量可以乘以任意标量，因此我们需要标准化向量以进行比较
    eigenvectors.col(0).normalize();
    Eigen::MatrixX<TypeParam> expectedVector(2, 1);
    expectedVector << 1/sqrt(2), 1/sqrt(2);
    for (int i = 0; i < eigenvectors.rows(); ++i) {
        ASSERT_NEAR(eigenvectors(i, 0), std::abs(expectedVector(i, 0)), this->tolerance);
    }
}
