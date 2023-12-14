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
#include <exceptions/SolverException.h>

template <typename Scalar>
EigenvalueSolver<Scalar> *createSolver(const std::string &solverName, ArgumentParser &parser)
{
    int maxIter = parser.exists("maxIter") ? std::stoi(parser.get("maxIter")) : 100000;
    Scalar tol = parser.exists("tol") ? std::stoi(parser.get("tol")) : 1e-6;
    Scalar shift = parser.exists("shift") ? std::stoi(parser.get("shift")) : 0;

    // If method is shifted, then shift must be provided
    if ((solverName == "pms" || solverName == "ims") && !parser.exists("shift"))
    {
        throw SolverInitializationException("Shift must be provided for shifted methods.",
                                            "Use --shift=<shift> to use a valid shift.");
    }

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
        throw SolverInitializationException("Invalid solver: " + solverName + ".",
                                            "Use --solver=<solver> to use a valid solver.\nThe available solvers are: qr, pm, im, pms, ims.");
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
        throw IOFileException("Failed to open file: " + filename,
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
        throw IOFileException("Failed to open file: " + filename,
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
        const Eigen::MatrixX<Scalar> &eigenvectors = solver->getEigenvectors();
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
        throw IOFileException("Failed to open file: " + filename,
                              "Make sure the results/ folder exists in the root directory.");
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

        bool isMatrixComplex = reader.isComplexMatrix();
        bool isSolverComplex = std::is_same<Scalar, std::complex<double>>::value;

        // Check if both matrix and solver are either complex or real
        if (isMatrixComplex != isSolverComplex)
        {
            std::string matrixType = isMatrixComplex ? "complex" : "real";
            std::string solverType = isSolverComplex ? "complex" : "real";

            throw SolverInitializationException("Matrix and solver types do not match. You are trying to use a " + solverType + " solver on a " + matrixType + " matrix.",
                                                "Use --type=" + matrixType + " to use the correct solver.");
        }

        solver->setMatrix(matrix);
        solver->solve();

        // save eigenvalues to file
        writeResultsToFile(solver, reader.getMatrixName());

        // save eigenvectors to file, qr method does not have eigenvectors
        writeEigenvectorsToFile(solver, reader.getMatrixName());

        // gnuploting the spectrum of eigenvalues (only one eigenvalue for power method)
        exportEigenvaluesToGnuplot(solver, reader.getMatrixName());
        std::string plotCommand = "gnuplot -e \"set terminal png size 800,600; "
                                  "set output 'results/" +
                                  reader.getMatrixName() + "_spectrum.png'; "
                                                           "set style circle radius graph 0.01; "
                                                           "plot 'results/" +
                                  reader.getMatrixName() + "_eigenvalues.dat' using 1:2 with circles fill solid lc rgb 'black'\"";
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
    try
    {
        ArgumentParser parser(argc, argv);

        // assert solver and matrix are provided
        if (!parser.exists("solver") || !parser.exists("matrix"))
        {

            throw SolverInitializationException("Both solver and matrix must be provided.",
                                                "Usage: build/Eigenvalues-PCSC --sovler=<solver> --matrix=<matrix> [options]");
        }

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
            throw SolverInitializationException("Invalid solver type: " + type + ".",
                                                "Use either --type=real or --type=complex.");
        }
    }
    catch (const SolverException &e)
    {
        std::cerr << "\033[31m[SOLVER ERROR]: " << e.what() << "\033[0m" << std::endl;
        return 1;
    }
    catch (const std::exception &e)
    {
        std::cerr << "\033[31m[ERROR]: " << e.what() << "\033[0m" << std::endl;
        return 1;
    }
    return 0;
}
