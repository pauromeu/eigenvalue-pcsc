//
// Created by Tachi on 2023/12/2.
//
#include <gtest/gtest.h>
#include <algorithms/PowerMethod.h>
#include "algorithms/PowerMethod.h"

// PowerMethod类的测试套件
template <typename Scalar>
class PowerMethodTest : public ::testing::Test {
protected:
    PowerMethod<Scalar> powerMethod; // PowerMethod实例

    void SetUp() override {
        // 如果需要，可以在这里进行初始化操作
    }

    void TearDown() override {
        // 清理代码（如果有的话）
    }
};

// 定义要使用的数据类型
using TestingTypes = ::testing::Types<float, double>;
TYPED_TEST_SUITE(PowerMethodTest, TestingTypes);

// 测试PowerMethod默认构造函数的行为
TYPED_TEST(PowerMethodTest, DefaultConstructor) {
    EXPECT_EQ(this->powerMethod.getMaxIterationsPublic(), 100);
    EXPECT_NEAR(this->powerMethod.getTolerancePublic(), static_cast<TypeParam>(1e-6), static_cast<TypeParam>(1e-8));
}

// 测试PowerMethod的solve方法
TYPED_TEST(PowerMethodTest, SolveMethod) {
    // 定义一个简单的矩阵，它的最大特征值是已知的
    Eigen::MatrixX<TypeParam> testMatrix(2, 2);
    testMatrix << 2, 0,
            0, 1;

    this->powerMethod.setMatrix(testMatrix);
    this->powerMethod.solve();

    // 获取计算出的最大特征值
    auto computedEigenvalues = this->powerMethod.getEigenvalues();

    // 验证算法是否能够正确地找到最大特征值
    EXPECT_NEAR(computedEigenvalues[0], static_cast<TypeParam>(2), static_cast<TypeParam>(1e-6));
}

// 可以根据需要添加更多测试用例
