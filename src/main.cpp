/**
 * Program name: Eigenvalues-PCSC
 * Description: A program that computes the eigenvalues and eigenvectors of a matrix using different methods. This program is part of the PCSC course at EPFL (MATH-458).
 * Authors: 1. Haolong Li 2. Pau Romeu Llordella
 * Date created: December 1, 2023
 * Last modified: December 15, 2023
 * Version: 1.0.0
 *
 * Usage: build/Eigenvalues-PCSC --sovler=<solver> --matrix=<matrix> [options]
 * Options:
 * --solver=<solver>       The solver to use. It can be one of the following: qr, pm, im, pms, ims.
 * --matrix=<matrix>       The matrix to use. The matrix must be in the data/matrix/ folder and must be in Matrix Market format. Only the name of the matrix is required, not the full path.
 * --type=<type>           The type of the matrix. It can be either real or complex. If not provided, it is assumed to be real.
 * --maxIter=<maxIter>     The maximum number of iterations. If not provided, it is assumed to be 100000.
 * --tol=<tol>             The tolerance. If not provided, it is assumed to be 1e-6.
 * --shift=<shift>         The shift to use for shifted methods. If not provided, it is assumed to be 0. It is compulsory for shifted methods.
 *
 * Examples:
 * 1. Compute the eigenvalues of the matrix can_24 using the QR method:
 * build/Eigenvalues-PCSC --solver=qr --matrix=can_24 --type=real --maxIter=1000 --tol=1e-6
 *
 * 2. Compute the greatest eigenvalue and its eigenvector of the matrix can_24 using the power method:
 * build/Eigenvalues-PCSC --solver=pm --matrix=can_24
 *
 * 3. Compute the smallest eigenvalue and its eigenvector of the matrix can_24 using the inverse power method:
 * build/Eigenvalues-PCSC --solver=im --matrix=can_24
 *
 * For more information, please refer to the README.md file.
 */

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
#include <utils/EigenExport.h>
#include <utils/EigenPlot.h>

/**
 * @brief Creates an instance of the solver based on the solver name provided.
 *
 * The solver parameters are also set.
 *
 * @tparam Scalar The scalar type of the solver. It can be either double or std::complex<double>.
 * @param solverName The name of the solver. It can be one of the following: qr, pm, im, pms, ims.
 * @param parser The argument parser used to get the solver parameters.
 *
 * @return A pointer to the solver instance.
 *
 * @throw SolverInitializationException If the solver name is invalid or the shift is not provided for shifted methods.
 */
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

    // Set solver parameters
    solver->setTolerance(tol);
    solver->setMaxIterations(maxIter);

    return solver;
}

/**
 * @brief Runs the solver on the matrix provided.
 *
 * The solver is run on the matrix provided by the matrixFile parameter.
 * The results are written to files and the spectrum of eigenvalues is plotted.
 *
 * @tparam Scalar The scalar type of the solver. It can be either double or std::complex<double>.
 * @param solver The solver instance.
 * @param matrixFile The path to the matrix file.
 *
 * @throw SolverInitializationException If the matrix and solver types do not match.
 * @throw IOFileSolverException If the results folder does not exist or the program does not have write permissions.
 */
template <typename Scalar>
void runSolver(EigenvalueSolver<Scalar> *solver, const std::string &matrixFile)
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

    // gnuploting the convergence of the method
    plotEigenvalues(reader.getMatrixName(), "eigenvalues");
    plotEigenvalues(reader.getMatrixName(), "spectrum");

    delete solver;
}

/**
 * @brief The main function of the program.
 *
 * The main function of the program. It parses the command line arguments and runs the solver.
 *
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 *
 * @return 0 if the program ran successfully, 1 otherwise.
 */
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
