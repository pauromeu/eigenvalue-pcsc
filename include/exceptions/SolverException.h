#include <stdexcept>
#include <string>

class SolverException : public std::runtime_error
{
public:
    explicit SolverException(const std::string &message, const std::string &tip = "")
        : std::runtime_error(message + (tip.empty() ? "" : "\nTip: " + tip)) {}
};

class SolverInitializationException : public SolverException
{
public:
    explicit SolverInitializationException(const std::string &message, const std::string &tip = "")
        : SolverException("Solver Initialization Error: " + message, tip) {}
};

class AlgebraException : public SolverException
{
public:
    explicit AlgebraException(const std::string &message, const std::string &tip = "")
        : SolverException("Algebra Error: " + message, tip) {}
};

class InvalidInputException2 : public SolverException
{
public:
    explicit InvalidInputException2(const std::string &message, const std::string &tip = "")
        : SolverException("Input Validation Error: " + message, tip) {}
};

class IterationLimitExceeded2 : public SolverException
{
public:
    explicit IterationLimitExceeded2(const std::string &message, const std::string &tip = "")
        : SolverException("Iteration Limit Exceeded: " + message, tip) {}
};

class NotImplementedException : public SolverException
{
public:
    explicit NotImplementedException(const std::string &message, const std::string &tip = "")
        : SolverException("Not Implemented: " + message, tip) {}
};

class IOFileException : public SolverException
{
public:
    explicit IOFileException(const std::string &message, const std::string &tip = "")
        : SolverException("File IO Error: " + message, tip) {}
};