#include <stdexcept>
#include <string>

/**
 * @class SolverException
 * @brief A class for exceptions thrown by the solvers.
 *
 * @note This class inherits from std::runtime_error.
 */
class SolverException : public std::runtime_error
{
public:
    /**
     * @brief Construct a new Solver Exception object
     *
     * @param message The message to be displayed
     * @param tip A tip to be displayed
     */
    explicit SolverException(const std::string &message, const std::string &tip = "")
        : std::runtime_error(message + (tip.empty() ? "" : "\nTip: " + tip)) {}
};

/**
 * @class SolverInitializationException
 * @brief A class for exceptions thrown by the solvers during initialization.
 * This class is used when a parameter is invalid, inconsistent or missing for the correct initialization of the solver.
 *
 * @note This class inherits from SolverException.
 */
class SolverInitializationException : public SolverException
{
public:
    /**
     * @brief Construct a new Solver Initialization Exception object
     *
     * @param message The message to be displayed
     * @param tip A tip to be displayed
     */
    explicit SolverInitializationException(const std::string &message, const std::string &tip = "")
        : SolverException("Solver Initialization Error: " + message, tip) {}
};

/**
 * @class AlgebraException
 * @brief A class for exceptions thrown by the solvers during algebraic operations.
 * This class is used when an algebraic operation fails.
 *
 * @note This class inherits from SolverException.
 */
class AlgebraException : public SolverException
{
public:
    /**
     * @brief Construct a new Algebra Exception object
     *
     * @param message The message to be displayed
     * @param tip A tip to be displayed
     */
    explicit AlgebraException(const std::string &message, const std::string &tip = "")
        : SolverException("Algebra Error: " + message, tip) {}
};

/**
 * @class InvalidInputException
 * @brief A class for exceptions thrown by the solvers during input validation.
 * This class is used when an input is invalid. For example, if the input matrix is not square.
 *
 * @note This class inherits from SolverException.
 */
class InvalidInputException : public SolverException
{
public:
    /**
     * @brief Construct a new Invalid Input Exception object
     *
     * @param message The message to be displayed
     * @param tip A tip to be displayed
     */
    explicit InvalidInputException(const std::string &message, const std::string &tip = "")
        : SolverException("Input Validation Error: " + message, tip) {}
};

/**
 * @class IterationLimitExceeded
 * @brief A class for exceptions thrown by the solvers when the maximum number of iterations is exceeded.
 *
 * @note This class inherits from SolverException.
 */
class IterationLimitExceeded : public SolverException
{
public:
    /**
     * @brief Construct a new Iteration Limit Exceeded object
     *
     * @param message The message to be displayed
     * @param tip A tip to be displayed
     */
    explicit IterationLimitExceeded(const std::string &message, const std::string &tip = "")
        : SolverException("Iteration Limit Exceeded: " + message, tip) {}
};

/**
 * @class NotImplementedSolverException
 * @brief A class for exceptions thrown by the solvers when a solver is not implemented.
 *
 * @note This class inherits from SolverException.
 */
class NotImplementedSolverException : public SolverException
{
public:
    /**
     * @brief Construct a new Not Implemented Solver Exception object
     *
     * @param message The message to be displayed
     * @param tip A tip to be displayed
     */
    explicit NotImplementedSolverException(const std::string &message, const std::string &tip = "")
        : SolverException("Not Implemented: " + message, tip) {}
};

/**
 * @class IOFileSolverException
 * @brief A class for exceptions thrown by the solvers when there is an error reading or writing a file.
 *
 * @note This class inherits from SolverException.
 */
class IOFileSolverException : public SolverException
{
public:
    /**
     * @brief Construct a new IO File Solver Exception object
     *
     * @param message The message to be displayed
     * @param tip A tip to be displayed
     */
    explicit IOFileSolverException(const std::string &message, const std::string &tip = "")
        : SolverException("File IO Error: " + message, tip) {}
};