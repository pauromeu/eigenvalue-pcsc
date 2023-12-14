#ifndef ARGUMENTPARSER_H
#define ARGUMENTPARSER_H

#include <string>
#include <map>

/**
 * @brief A simple argument parser for command line arguments.
 *
 * A simple argument parser for command line arguments. It parses the command line arguments and stores them in a map.
 *
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 */
class ArgumentParser
{
public:
    /**
     * @brief Construct a new Argument Parser object.
     *
     * Stores the command line arguments in a map. The arguments must be in the form "--key=value". It does not support any other format.
     *
     * @param argc The number of command line arguments.
     * @param argv The command line arguments.
     */
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

    /**
     * @brief Get the value of a command line argument.
     *
     * If the argument does not exist, it returns an empty string.
     *
     * @param key The key of the argument.
     * @return The value of the argument.
     */
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

    /**
     * @brief Check if a command line argument exists.
     *
     * @param key The key of the argument.
     * @return True if the argument exists, false otherwise.
     */
    bool exists(const std::string &key) const
    {
        return args.find(key) != args.end();
    }

private:
    std::map<std::string, std::string> args;
};

#endif // ARGUMENTPARSER_H