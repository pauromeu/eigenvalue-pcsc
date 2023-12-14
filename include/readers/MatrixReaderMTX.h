#ifndef MATRIX_READER_MTX_H
#define MATRIX_READER_MTX_H

#include <Eigen/Sparse>
#include <Eigen/Dense>

/**
 * @brief A class for reading matrices in the Matrix Market format (.mtx).
 *
 * It supports both real and complex matrices. It can return the matrix as a dense or sparse matrix.
 *
 * The Matrix Market format is a simple format for storing matrices. If you want to know more about it, check out the [Matrix Market website](https://math.nist.gov/MatrixMarket/formats.html).
 *
 * @tparam Scalar The scalar type of the matrix.
 */
template <typename Scalar>
class MatrixReaderMTX
{
public:
    /**
     * @brief Construct a new Matrix Reader MTX object.
     *
     * @param filename The path to the matrix file.
     */
    MatrixReaderMTX(const std::string &filename)
    {
        parseFile(filename);
    }

    /**
     * @brief Get the matrix as a dense matrix.
     *
     * @return The matrix as a dense matrix.
     */
    Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> getDense()
    {
        return Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic>(matrix);
    }

    /**
     * @brief Get the matrix as a sparse matrix.
     *
     * @return The matrix as a sparse matrix.
     */
    Eigen::SparseMatrix<Scalar> getSparse()
    {
        return matrix;
    }

    /**
     * @brief Print the metadata of the matrix.
     *
     * The metadata includes the name, dimensions, number of non-zero elements, and whether the matrix is complex or symmetric.
     */
    void printMetadata()
    {
        std::cout << "====== Matrix: " + name + " ======" << std::endl;
        std::cout << "Rows: " << rows << ", Cols: " << cols << ", NNZ: " << nnz << std::endl;
        std::cout << std::boolalpha
                  << "Is complex: " << isComplex
                  << ", Is symmetric: " << isSymmetric << std::endl;
        std::cout << "=========================" << std::endl;
    }

    /**
     * @brief Check if the matrix is complex.
     *
     * @return True if the matrix is complex, false otherwise.
     */
    bool isComplexMatrix()
    {
        return isComplex;
    }

    /**
     * @brief Get the name of the matrix.
     *
     * @return The name of the matrix.
     */
    std::string getMatrixName()
    {
        return name;
    }

private:
    Eigen::SparseMatrix<Scalar> matrix;
    std::string name;
    bool isSymmetric;
    bool isComplex;
    int rows, cols, nnz;

    /**
     * @brief Parse the matrix file.
     *
     * @param filename The path to the matrix file.
     *
     * @throw IOFileSolverException If the file does not exist or the extension is not .mtx or the matrix is not in the Matrix Market format.
     */
    void parseFile(const std::string &filename)
    {
        std::ifstream file(filename);

        // Assert file is open
        if (!file.is_open())
        {
            throw IOFileSolverException("Failed to open file: " + filename,
                                        "Make sure the matrix is in the data/matrix/ folder.");
        }

        // Assert file extension is .mtx
        if (filename.substr(filename.find_last_of(".") + 1) != "mtx")
        {
            throw IOFileSolverException("Invalid file extension: " + filename,
                                        "Make sure the matrix is in the Matrix Market format (.mtx).");
        }

        name = filename.substr(filename.find_last_of("/") + 1, filename.find_last_of(".") - filename.find_last_of("/") - 1);

        processHeader(file);

        readMatrix(file);

        file.close();
    }

    /**
     * @brief Process the header of the matrix file.
     *
     * The header contains the dimensions and whether the matrix is complex or symmetric.
     *
     * @param file The matrix file.
     */
    void processHeader(std::ifstream &file)
    {
        // Get first line and look for "complex" or "symmetric"
        std::string line;
        std::getline(file, line);

        isComplex = false;
        isSymmetric = false;
        if (line.find("complex") != std::string::npos)
        {
            isComplex = true;
        }
        if (line.find("symmetric") != std::string::npos)
        {
            isSymmetric = true;
        }

        // Skip rest of lines starting with %
        while (line[0] == '%')
        {
            std::getline(file, line);
        }

        // Get dimensions (first line after % comments)
        std::stringstream ss(line);
        ss >> rows >> cols >> nnz;
    }

    /**
     * @brief Read the matrix from the file.
     *
     * It handles both real and complex matrices and stores them in a sparse matrix.
     *
     * @param file The matrix file.
     */
    void readMatrix(std::ifstream &file)
    {
        matrix = Eigen::SparseMatrix<Scalar>(rows, cols);

        // Read each line
        std::string line;
        while (std::getline(file, line))
        {
            // Skip comments
            if (line[0] == '%')
            {
                continue;
            }

            // Parse line
            std::stringstream ss(line);
            int row, col;
            double real, imag;

            if (isComplex)
            {
                ss >> row >> col >> real >> imag;
                addComplexEntry(row, col, real, imag);
            }
            else
            {
                ss >> row >> col >> real;
                addRealEntry(row, col, real);
            }
        }
    }

    /**
     * @brief Add a complex entry to the matrix.
     *
     * @param row The row of the entry.
     * @param col The column of the entry.
     * @param real The real part of the entry.
     * @param imag The imaginary part of the entry.
     */
    void addComplexEntry(int row, int col, double real, double imag);

    /**
     * @brief Add a real entry to the matrix.
     *
     * @param row The row of the entry.
     * @param col The column of the entry.
     * @param real The real part of the entry.
     */
    void addRealEntry(int row, int col, double real);
};

template <>
void MatrixReaderMTX<std::complex<double>>::addComplexEntry(int row, int col, double real, double imag)
{
    matrix.insert(row - 1, col - 1) = std::complex<double>(real, imag);
    if (isSymmetric && row != col)
    {
        matrix.insert(col - 1, row - 1) = std::complex<double>(real, -imag); // Negation of imaginary part for Hermitian matrix
    }
}

template <>
void MatrixReaderMTX<double>::addComplexEntry(int row, int col, double real, double imag) {}

template <>
void MatrixReaderMTX<double>::addRealEntry(int row, int col, double real)
{
    matrix.insert(row - 1, col - 1) = real;
    if (isSymmetric && row != col)
    {
        matrix.insert(col - 1, row - 1) = real;
    }
}

template <>
void MatrixReaderMTX<std::complex<double>>::addRealEntry(int row, int col, double real) {}

#endif // MATRIX_READER_MTX_H