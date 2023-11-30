#ifndef MTXMATRIXREADER_H
#define MTXMATRIXREADER_H

#include "MatrixReader.h"
#include <Eigen/Sparse>
#include <Eigen/Dense>
#include <string>

class MTXMatrixReader : public MatrixReader
{
public:
    MTXMatrixReader(const std::string &filePath);

    void readMatrix() override;

    Eigen::SparseMatrix<double> getSparseMatrix() const override;

    Eigen::MatrixXd getDenseMatrix() const override;

private:
    std::string filePath;
    Eigen::SparseMatrix<double> matrix;
};

#endif // MTXMATRIXREADER_H
