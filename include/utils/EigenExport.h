#ifndef EIGEN_EXPORT_H
#define EIGEN_EXPORT_H

#include <string>
#include <algorithms/EigenvalueSolver.h>

template <typename Scalar>
void writeResultsToFile(const EigenvalueSolver<Scalar> *solver, const std::string &matrixName)
{
    std::string filename = "results/" + matrixName + "_values.txt";
    std::ofstream file(filename);
    if (file.is_open())
    {
        file << solver->getEigenvalues();
        file.close();
        std::cout << "Results written to file: " << filename << std::endl;
    }
    else
    {
        throw IOFileSolverException("Failed to open file: " + filename,
                                    "Make sure the results/ folder exists in the root directory.");
    }
}

template <typename Scalar>
void exportEigenvaluesToGnuplot(const EigenvalueSolver<Scalar> *solver, const std::string &matrixName)
{
    std::string filename = "results/" + matrixName + "_eigenvalues.dat";
    std::ofstream file(filename);
    if (file.is_open())
    {
        auto eigenvalues = solver->getEigenvalues();
        for (int i = 0; i < eigenvalues.size(); ++i)
        {
            file << eigenvalues[i].real() << " " << eigenvalues[i].imag() << std::endl;
        }
        file.close();
    }
    else
    {
        throw IOFileSolverException("Failed to open file: " + filename,
                                    "Make sure the results/ folder exists in the root directory.");
    }
}

template <typename Scalar>
void writeEigenvectorsToFile(const EigenvalueSolver<Scalar> *solver, const std::string &matrixName)
{
    std::string filename = "results/" + matrixName + "_vectors.txt";
    std::ofstream file(filename);
    if (file.is_open())
    {
        Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> eigenvectors;
        try
        {
            eigenvectors = solver->getEigenvectors();
        }
        catch (const NotImplementedSolverException &e) // QR method does not compute eigenvectors, don't write to file but don't throw exception
        {
            std::cout << "[WARNING]: This solver does not compute eigenvectors." << std::endl;
            return;
        }

        for (int i = 0; i < eigenvectors.rows(); ++i)
        {
            for (int j = 0; j < eigenvectors.cols(); ++j)
            {
                if constexpr (std::is_same<Scalar, std::complex<double>>::value)
                {
                    file << eigenvectors(i, j).real() << "+" << eigenvectors(i, j).imag() << "i";
                }
                else
                {
                    file << eigenvectors(i, j);
                }
                if (j < eigenvectors.cols() - 1)
                    file << " ";
            }
            file << std::endl;
        }
        file.close();
        std::cout << "Eigenvectors written to file: " << filename << std::endl;
    }
    else
    {
        throw IOFileSolverException("Failed to open file: " + filename,
                                    "Make sure the results/ folder exists in the root directory.");
    }
}

#endif // EIGEN_EXPORT_H