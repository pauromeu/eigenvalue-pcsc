#include "matrix_readers/MTXMatrixReader.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>

MTXMatrixReader::MTXMatrixReader(const std::string &filePath)
    : filePath(filePath), matrix(0, 0) {}

void MTXMatrixReader::readMatrix()
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        throw std::runtime_error("Unable to open file: " + filePath);
    }

    std::string line;
    bool headerPassed = false;
    int rows, cols, nonZeros;
    std::vector<Eigen::Triplet<double>> tripletList;

    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '%')
            continue; // Skip comments

        if (!headerPassed)
        {
            std::istringstream iss(line);
            iss >> rows >> cols >> nonZeros;
            matrix.resize(rows, cols);
            headerPassed = true;
        }
        else
        {
            std::istringstream iss(line);
            int row, col;
            iss >> row >> col;
            tripletList.emplace_back(row - 1, col - 1, 1.0); // Adjusting indices to 0-based
            if (row != col)                                  // If not a diagonal element (symmetric matrix)
                tripletList.emplace_back(col - 1, row - 1, 1.0);
        }
    }

    matrix.setFromTriplets(tripletList.begin(), tripletList.end());
}

Eigen::SparseMatrix<double> MTXMatrixReader::getSparseMatrix() const
{
    return matrix;
}

Eigen::MatrixXd MTXMatrixReader::getDenseMatrix() const
{
    return Eigen::MatrixXd(matrix);
}
