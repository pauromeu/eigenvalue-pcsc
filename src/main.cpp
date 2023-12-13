#include <iostream>
#include <map>
#include <string>
#include <algorithms/EigenvalueSolver.h>
#include <algorithms/QRMethod.h>
#include <algorithms/PowerMethod.h>
#include <algorithms/InversePowerMethod.h>
#include <algorithms/PowerMethodWithShift.h>
#include <algorithms/InversePowerMethodWithShift.h>
#include <fstream>
#include <readers/MatrixReaderMTX.h>
#include <readers/ArgumentParser.h>

template <typename Scalar>
EigenvalueSolver<Scalar> *createSolver(const std::string &solverName, ArgumentParser &parser)
{
    int maxIter = parser.exists("maxIter") ? std::stoi(parser.get("maxIter")) : 100000;
    Scalar tol = parser.exists("tol") ? std::stoi(parser.get("tol")) : 1e-6;
    Scalar shift = parser.exists("shift") ? std::stoi(parser.get("shift")) : 0;

    EigenvalueSolver<Scalar> *solver = nullptr;

    if (solverName == "qr")
    {
        solver = new QRMethod<Scalar>();
    }
    else if (solverName == "pm") // Power method
    {
        solver = new PowerMethod<Scalar>();
    }
    else if (solverName == "im") // Inverse power method
    {
        solver = new InversePowerMethod<Scalar>();
    }
    else if (solverName == "pms") // Power method with shift
    {
        auto shiftedSolver = new PowerMethodWithShift<Scalar>();
        shiftedSolver->setShift(shift);
        solver = shiftedSolver;
    }
    else if (solverName == "ims") // Inverse power method with shift
    {
        auto shiftedSolver = new InversePowerMethodWithShift<Scalar>();
        shiftedSolver->setShift(shift);
        solver = shiftedSolver;
    }
    else
    {
        std::cout << "Invalid solver name." << std::endl;
        return nullptr;
    }

    solver->setTolerance(tol);
    solver->setMaxIterations(maxIter);

    return solver;
}

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
        std::cerr << "Failed to open file: " << filename << std::endl;
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
        std::cerr << "Failed to open file: " << filename << std::endl;
    }
}

template <typename Scalar>
void writeEigenvectorsToFile(const EigenvalueSolver<Scalar> *solver, const std::string &matrixName)
{
    std::string filename = "results/" + matrixName + "_vectors.txt";
    std::ofstream file(filename);
    if (file.is_open())
    {
        const Eigen::MatrixX<Scalar>& eigenvectors = solver->getEigenvectors();
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
        std::cerr << "Failed to open file: " << filename << std::endl;
    }
}

template <typename Scalar>
void runSolver(EigenvalueSolver<Scalar> *solver, const std::string &matrixFile)
{
    try
    {
        MatrixReaderMTX<Scalar> reader(matrixFile);
        auto matrix = reader.getDense();
        reader.printMetadata();
/*
        Emergency fix to matrix reading:
        TODO: fix problem with reading matrix from file
        matrix.resize(2,2);
        matrix << 1, 2,
                2, 1;
*/
        solver->setMatrix(matrix);
        solver->solve();

        // save eigenvalues to file
        writeResultsToFile(solver, reader.getMatrixName());

        // save eigenvectors to file, qr method does not have eigenvectors
        writeEigenvectorsToFile(solver, reader.getMatrixName());

        // gnuploting the spectrum of eigenvalues (only one eigenvalue for power method)
        exportEigenvaluesToGnuplot(solver, reader.getMatrixName());
        std::string plotCommand = "gnuplot -e \"set terminal png size 800,600; "
                                  "set output 'results/" + reader.getMatrixName() + "_spectrum.png'; "
                                                                                    "set style circle radius graph 0.01; "
                                                                                    "plot 'results/" + reader.getMatrixName() + "_eigenvalues.dat' using 1:2 with circles fill solid lc rgb 'black'\"";
        system(plotCommand.c_str());
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error in runSolver: " << e.what() << std::endl;
    }
    delete solver;
}

int main(int argc, char **argv)
{
    ArgumentParser parser(argc, argv);

    std::string solverName = parser.get("solver");
    std::string matrixFile = "data/matrix/" + parser.get("matrix") + ".mtx";
    std::string type = parser.exists("type") ? parser.get("type") : "real";

    if (type == "real")
    {
        auto solver = createSolver<double>(solverName, parser);
        runSolver(solver, matrixFile);
    }
    else if (type == "complex")
    {
        auto solver = createSolver<std::complex<double>>(solverName, parser);
        runSolver(solver, matrixFile);
    }
    else
    {
        std::cout << "Invalid type." << std::endl;
    }
    return 0;
}
