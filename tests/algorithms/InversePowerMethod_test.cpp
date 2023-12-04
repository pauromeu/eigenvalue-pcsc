#include <gtest/gtest.h>
#include <algorithms/InversePowerMethod.h>
#include <Eigen/Dense>

// InversePowerMethod类的测试套件
template <typename Scalar>
class InversePowerMethodTest : public ::testing::Test {
protected:
    InversePowerMethod<Scalar> solver;
    Eigen::MatrixX<Scalar> matrix;
    Scalar tolerance;
    int maxIterations;

    void SetUp() override {
        // 设置测试中使用的矩阵、公差和最大迭代次数
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

// 使用 double 和 float 类型实例化测试
typedef ::testing::Types<double, float> Implementations;
TYPED_TEST_SUITE(InversePowerMethodTest, Implementations);

TYPED_TEST(InversePowerMethodTest, SolvesCorrectly) {
    this->solver.solve();

    // 获取结果
    Eigen::MatrixX<TypeParam> eigenvectors = this->solver.getEigenvectors();
    Eigen::VectorX<TypeParam> eigenvalues = this->solver.getEigenvalues();

    // 验证特征值
    ASSERT_NEAR(eigenvalues(0), (11 - sqrt(17))/2, this->tolerance); // 假设已知的最小特征值

    // 验证特征向量
    // 注意：特征向量可以乘以任意标量，因此我们需要标准化向量以进行比较
    eigenvectors.col(0).normalize();

    Eigen::MatrixX<TypeParam> expectedVector(2, 1);
    expectedVector << (-1 - sqrt(17)) / 4, 1;
    expectedVector.normalize();

    for (int i = 0; i < eigenvectors.rows(); ++i) {
        ASSERT_NEAR(std::abs(eigenvectors(i, 0)), std::abs(expectedVector(i, 0)), this->tolerance);
    }
}
