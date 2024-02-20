#include "IO.h"

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>

namespace IO {

    std::string readFile(const std::string& path)
    {
        using namespace std::string_literals;
        std::ifstream t(path);
        if (t.fail())
        {
            std::cout << "Could not open "s + path << std::endl;
        }
        std::stringstream buffer;
        buffer << t.rdbuf();
        auto result = buffer.str();
        result.erase(std::remove(result.begin(), result.end(), '\r'), result.end());
        return result;
    }
}
