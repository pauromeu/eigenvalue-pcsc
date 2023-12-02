//
// Created by Tachi on 2023/12/2.
//
#include <algorithms/PowerMethod.h>
#include <Eigen/Dense>

template <typename Scalar>
PowerMethod<Scalar>::PowerMethod(int maxIterations, Scalar tolerance) {
    this->setMaxIterations(maxIterations); // 设置最大迭代次数
    this->setTolerance(tolerance);         // 设置容忍度
}

template <typename Scalar>
PowerMethod<Scalar>::~PowerMethod() {
    // 析构函数的实现（如果需要）
}

template <typename Scalar>
void PowerMethod<Scalar>::setMatrix(const Eigen::MatrixX<Scalar> &matrix) {
    this->setMatrix(matrix); // 调用基类的setMatrix方法
}

template <typename Scalar>
void PowerMethod<Scalar>::solve() {
    Eigen::VectorX<Scalar> x = Eigen::VectorX<Scalar>::Random(this->getMatrix().rows());
    Scalar lambda = 0;
    Scalar lambda_prev;

    int maxIterations = this->getMaxIterations();
    Scalar tolerance = this->getTolerance();

    for (int i = 0; i < maxIterations; ++i) {
        lambda_prev = lambda;
        Eigen::VectorX<Scalar> y = this->getMatrix() * x;
        x = y.normalized();
        lambda = x.transpose() * this->getMatrix() * x;

        if (std::abs(lambda - lambda_prev) < tolerance) {
            break;
        }
    }

    this->setEigenvalues(Eigen::VectorX<Scalar>::Constant(1, lambda));
    this->setEigenvectors(x);
}

// 显式实例化模板
template class PowerMethod<double>;
template class PowerMethod<float>;

// 如果需要其他类型，也可以在此处添加

