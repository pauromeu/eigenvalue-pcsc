#include <string>
#include <map>

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