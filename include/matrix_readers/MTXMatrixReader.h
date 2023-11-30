#ifndef MTXMATRIXREADER_H
#define MTXMATRIXREADER_H

#include "MatrixReader.h"
#include <Eigen/Sparse>
#include <Eigen/Dense>
#include <string>

/**
 * @class MTXMatrixReader
 * @brief Reader class for parsing matrices from MTX (Matrix Market) files.
 *
 * This class extends MatrixReader and provides an implementation for
 * reading matrices stored in MTX format. Currently, only symmetric matrices are supported.
 */
class MTXMatrixReader : public MatrixReader
{
public:
    /**
     * @brief Construct a new MTX Matrix Reader object.
     *
     * Initializes the reader with the path to the MTX file.
     * @param filePath Path to the MTX file to be read.
     */
    MTXMatrixReader(const std::string &filePath);

    /**
     * @brief Reads and parses the matrix from the MTX file.
     *
     * The function reads the MTX file specified by filePath, parsing its
     * content to construct the matrix in Eigen's SparseMatrix format. It
     * handles the differences in MTX formats internally.
     */
    void readMatrix() override;

    /**
     * @brief Get the Matrix in Eigen's SparseMatrix format.
     *
     * @return Eigen::SparseMatrix<double> The parsed matrix in sparse format.
     */
    Eigen::SparseMatrix<double> getSparseMatrix() const override;

    /**
     * @brief Get the Matrix in Eigen's DenseMatrix format.
     *
     * @return Eigen::MatrixXd The parsed matrix in dense format.
     */
    Eigen::MatrixXd getDenseMatrix() const override;

private:
    std::string filePath;
    Eigen::SparseMatrix<double> matrix;
};

#endif // MTXMATRIXREADER_H
