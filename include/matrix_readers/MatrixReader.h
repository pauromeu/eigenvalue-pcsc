#ifndef MATRIXREADER_H
#define MATRIXREADER_H

#include <Eigen/Sparse>

class MatrixReader
{
public:
    virtual ~MatrixReader() {}

    virtual void readMatrix() = 0;

    virtual Eigen::SparseMatrix<double> getSparseMatrix() const = 0;

    virtual Eigen::MatrixXd getDenseMatrix() const = 0;
};

#endif // MATRIXREADER_H