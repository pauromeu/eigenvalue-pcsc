#include <Eigen/Sparse>
#include <Eigen/Dense>

template <typename Scalar>
class MatrixReaderMTX
{
public:
    MatrixReaderMTX(const std::string &filename)
    {
        parseFile(filename);
    }

    Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> getDense()
    {
        return Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic>(matrix);
    }

    Eigen::SparseMatrix<Scalar> getSparse()
    {
        return matrix;
    }

    void printMetadata()
    {
        std::cout << "====== Matrix: " + name + " ======" << std::endl;
        std::cout << "Rows: " << rows << ", Cols: " << cols << ", NNZ: " << nnz << std::endl;
        std::cout << std::boolalpha
                  << "Is complex: " << isComplex
                  << ", Is symmetric: " << isSymmetric << std::endl;
        std::cout << "=========================" << std::endl;
    }

    bool isComplexMatrix()
    {
        return isComplex;
    }

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

    void parseFile(const std::string &filename)
    {
        std::ifstream file(filename);

        // Assert file is open
        if (!file.is_open())
        {
            throw IOFileException("Failed to open file: " + filename,
                                  "Make sure the matrix is in the data/matrix/ folder.");
        }

        // Assert file extension is .mtx
        if (filename.substr(filename.find_last_of(".") + 1) != "mtx")
        {
            throw IOFileException("Invalid file extension: " + filename,
                                  "Make sure the matrix is in the Matrix Market format (.mtx).");
        }

        name = filename.substr(filename.find_last_of("/") + 1, filename.find_last_of(".") - filename.find_last_of("/") - 1);

        processHeader(file);

        readMatrix(file);

        file.close();
    }

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

    void addComplexEntry(int row, int col, double real, double imag);
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
