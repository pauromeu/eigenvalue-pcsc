#include <iostream>
#include <map>
#include <string>
#include <algorithm>
#include <algorithms/EigenvalueSolver.h>
#include <algorithms/QRMethod.h>
#include <algorithms/PowerMethod.h>
#include <algorithms/InversePowerMethod.h>
#include <algorithms/PowerMethodWithShift.h>
#include <algorithms/InversePowerMethodWithShift.h>
#include <matrix_readers/MTXMatrixReader.h>
#include <fstream>

class ArgumentParser
{
public:
    ArgumentParser(int &argc, char **argv)
    {
        for (int i = 1; i < argc; ++i)
        {
            std::string token = argv[i];
            if (token.substr(0, 2) == "--")
            {
                token = token.substr(2); // Remove the "--" prefix
                size_t pos = token.find('=');
                if (pos != std::string::npos)
                {
                    std::string key = token.substr(0, pos);
                    std::string value = token.substr(pos + 1);
                    args[key] = value;
                }
            }
        }
    }

    const std::string &get(const std::string &key) const
    {
        auto itr = args.find(key);
        if (itr != args.end())
        {
            return itr->second;
        }
        static const std::string empty_string("");
        return empty_string;
    }

    bool exists(const std::string &key) const
    {
        return args.find(key) != args.end();
    }

private:
    std::map<std::string, std::string> args;
};

EigenvalueSolver<double> *createRealSolver(const std::string &solverName, ArgumentParser &parser)
{
    int maxIter = parser.exists("maxIter") ? std::stoi(parser.get("maxIter")) : 100000;
    double tol = parser.exists("tol") ? std::stoi(parser.get("tol")) : 1e-6;
    double shift = parser.exists("shift") ? std::stoi(parser.get("shift")) : 0;

    EigenvalueSolver<double> *solver = nullptr;

    if (solverName == "qr")
    {
        solver = new QRMethod<double>();
    }
    else if (solverName == "pm") // Power method
    {
        solver = new PowerMethod<double>();
    }
    else if (solverName == "im") // Inverse power method
    {
        solver = new InversePowerMethod<double>();
    }
    else if (solverName == "pms") // Power method with shift
    {
        auto shiftedSolver = new PowerMethodWithShift<double>();
        shiftedSolver->setShift(shift);
        solver = shiftedSolver;
    }
    else if (solverName == "ims") // Inverse power method with shift
    {
        auto shiftedSolver = new InversePowerMethodWithShift<double>();
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

EigenvalueSolver<std::complex<double>> *createComplexSolver(const std::string &solverName, ArgumentParser &parser)
{
    int maxIter = parser.exists("maxIter") ? std::stoi(parser.get("maxIter")) : 100000;
    std::complex<double> tol = parser.exists("tol") ? std::stoi(parser.get("tol")) : 1e-6;
    std::complex<double> shift = parser.exists("shift") ? std::stoi(parser.get("shift")) : 0;

    EigenvalueSolver<std::complex<double>> *solver = nullptr;

    if (solverName == "qr")
    {
        solver = new QRMethod<std::complex<double>>();
    }
    else if (solverName == "pm") // Power method
    {
        solver = new PowerMethod<std::complex<double>>();
    }
    else if (solverName == "im") // Inverse power method
    {
        solver = new InversePowerMethod<std::complex<double>>();
    }
    else if (solverName == "pms") // Power method with shift
    {
        auto shiftedSolver = new PowerMethodWithShift<std::complex<double>>();
        shiftedSolver->setShift(shift);
        solver = shiftedSolver;
    }
    else if (solverName == "ims") // Inverse power method with shift
    {
        auto shiftedSolver = new InversePowerMethodWithShift<std::complex<double>>();
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

int main(int argc, char **argv)
{
    ArgumentParser parser(argc, argv);

    std::string solverName = parser.get("solver");
    std::string matrixFile = parser.get("matrix");

    // If the matrix typs is not specified, assume it is real
    std::string matrixType = parser.exists("matrixType") ? parser.get("matrixType") : "real";

    EigenvalueSolver<double> *solver = createRealSolver(solverName, parser);

    auto reader = MTXMatrixReader(matrixFile);
    reader.readMatrix();
    Eigen::MatrixXd matrix = reader.getDenseMatrix();

    solver->setMatrix(matrix);
    solver->solve();

    std::cout << "Eigenvalue(s): " << std::endl;
    std::cout << solver->getEigenvalues() << std::endl;

    std::ofstream file(parser.get("matrix") + "_values.txt");
    if (file.is_open())
    {
        file << solver->getEigenvalues();
    }
    file.close();

    delete solver;

    return 0;
}
