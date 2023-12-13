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
void runSolver(EigenvalueSolver<Scalar> *solver, const std::string &matrixFile)
{
    try
    {
        MatrixReaderMTX<Scalar> reader(matrixFile);
        auto matrix = reader.getDense();
        reader.printMetadata();
        solver->setMatrix(matrix);
        solver->solve();

        writeResultsToFile(solver, reader.getMatrixName());
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
