#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "fundamentum/Logger.h"

#include "IO.h"

namespace IO {

    std::string readFile(const std::string& path)
    {
        using namespace std::string_literals;
        std::ifstream t(path);
        if (t.fail())
        {
            Log::log() << "Could not open " << path;
        }
        std::stringstream buffer;
        buffer << t.rdbuf();
        auto result = buffer.str();
        result.erase(std::remove(result.begin(), result.end(), '\r'), result.end());
        return result;
    }
}
