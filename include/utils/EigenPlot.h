#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include "exceptions/SolverException.h"

void plotEigenvalues(const std::string &matrixName, const std::string &scriptName)
{
    const std::string scriptFolder = "gnuplot";
    const std::string scriptFilename = scriptFolder + "/plot_" + scriptName + ".gp";
    const std::string dataFilename = "results/" + matrixName + "_eigenvalues.dat";
    const std::string outputFilename = "results/" + matrixName + "_" + scriptName + ".png";

    std::ifstream scriptTemplate(scriptFilename);
    if (!scriptTemplate.is_open())
    {
        throw IOFileSolverException("Failed to open Gnuplot script template: " + scriptFilename,
                                    "Ensure the script file exists and the path is correct.");
    }

    std::stringstream buffer;
    buffer << scriptTemplate.rdbuf();
    scriptTemplate.close();

    std::string scriptContent = buffer.str();

    size_t pos;
    while ((pos = scriptContent.find("OUTPUT_FILE")) != std::string::npos)
    {
        scriptContent.replace(pos, std::string("OUTPUT_FILE").length(), outputFilename);
    }
    while ((pos = scriptContent.find("DATA_FILE")) != std::string::npos)
    {
        scriptContent.replace(pos, std::string("DATA_FILE").length(), dataFilename);
    }

    // Write modified script to a temporary file
    std::string tempScriptFilename = scriptFolder + "/temp.gp";
    std::ofstream tempScript(tempScriptFilename);
    if (!tempScript.is_open())
    {
        throw IOFileSolverException("Failed to open temporary Gnuplot script file: " + tempScriptFilename,
                                    "Check if the program has write permissions in the directory.");
    }
    tempScript << scriptContent;
    tempScript.close();

    // Execute the Gnuplot script
    std::string plotCommand = "gnuplot " + tempScriptFilename;

    int result = system(plotCommand.c_str());
    if (result != 0)
    {
        throw IOFileSolverException("Failed to execute Gnuplot script.",
                                    "Ensure Gnuplot is installed and accessible from the command line.");
    }

    std::remove(tempScriptFilename.c_str());
}
