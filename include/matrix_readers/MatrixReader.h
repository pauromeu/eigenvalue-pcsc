#ifndef MATRIXREADER_H
#define MATRIXREADER_H

#include <Eigen/Sparse>

/**
 * @brief Abstract base class for matrix readers.
 *
 * Defines the interface for reading matrices from various file formats.
 */
class MatrixReader
{
public:
    virtual ~MatrixReader() {}

    /**
     * @brief Reads the matrix from the file.
     */
    virtual void readMatrix() = 0;

    /**
     * @brief Retrieve the Eigen sparse matrix.
     *
     * @return Eigen::SparseMatrix<double> The matrix in sparse format.
     */
    virtual Eigen::SparseMatrix<double> getSparseMatrix() const = 0;

    /**
     * @brief Retrieve the Eigen dense matrix.
     *
     * @return Eigen::MatrixXd The matrix in dense format.
     */
    virtual Eigen::MatrixXd getDenseMatrix() const = 0;
};

#endif // MATRIXREADER_H