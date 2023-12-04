#include <gtest/gtest.h>
#include <algorithms/InversePowerMethodWithShift.h>
#include <Eigen/Dense>

// PowerMethodWithShift 类的测试套件
template <typename Scalar>
class InversePowerMethodWithShiftTest : public ::testing::Test {
protected:
    InversePowerMethodWithShift<Scalar> solver;
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
        shift = 5.0;  // 选择一个适当的位移量

        solver.setShift(shift);
        solver.setMatrix(matrix);
        solver.setTolerance(tolerance);
        solver.setMaxIterations(maxIterations);
    }
};

// 使用 double 和 float 类型实例化测试
typedef ::testing::Types<double, float> Implementations;
TYPED_TEST_SUITE(InversePowerMethodWithShiftTest, Implementations);

TYPED_TEST(InversePowerMethodWithShiftTest, SolvesCorrectly) {
    this->solver.solve();

    // 获取结果
    Eigen::MatrixX<TypeParam> eigenvectors = this->solver.getEigenvectors();
    Eigen::VectorX<TypeParam> eigenvalues = this->solver.getEigenvalues();

    // 验证特征值
    ASSERT_NEAR(eigenvalues(0), 3.0, this->tolerance);

    // 验证特征向量 - 使用余弦相似度
    Eigen::VectorX<TypeParam> actualVector = eigenvectors.col(0); // 转换为向量类型
    Eigen::VectorX<TypeParam> expectedVector(2);
    expectedVector << 1/sqrt(2), 1/sqrt(2);

    TypeParam cosSim = actualVector.normalized().dot(expectedVector.normalized());
    ASSERT_NEAR(std::abs(cosSim), 1.0, this->tolerance);
}

