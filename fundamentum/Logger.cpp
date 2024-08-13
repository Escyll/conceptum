#include <string>
#include <vector>
#include <iostream>

#include "Logger.h"

namespace Log {

struct LoggerContext
{
    std::vector<std::string> logLines;
    std::string soFar;
};
LoggerContext* gContext = nullptr;

void setContext(LoggerContext* context)
{
    gContext = context;
}

LoggerContext* context()
{
    return gContext;
}

LoggerContext* createContext()
{
    gContext = new LoggerContext;
    return gContext;
}

void destroyContext(LoggerContext* context)
{
    delete context;
}

Log& Log::operator<<(const char* value)
{
    gContext->soFar += value;
    return *this;
}

Log& Log::operator<<(const std::string& value)
{
    gContext->soFar += value;
    return *this;
}

Log& Log::operator<<(const std::filesystem::path& value)
{
    gContext->soFar += value.string();
    return *this;
}

template<typename T>
Log& Log::operator<<(T value)
{
    gContext->soFar += std::to_string(value);
    return *this;
}

template Log& Log::operator<<(float);
template Log& Log::operator<<(int);
template Log& Log::operator<<(double);
template Log& Log::operator<<(unsigned int);
template Log& Log::operator<<(long);
template Log& Log::operator<<(long long);

Log::~Log()
{
    gContext->logLines.push_back(gContext->soFar);
}

Log log()
{
    gContext->soFar = "";
    return Log();
}

std::vector<std::string> lastNLogLines(unsigned int maxLines)
{
    std::vector<std::string> result;
    int nLines = gContext->logLines.size();
    for (int i = nLines - 1; i > std::max(nLines - 1 - static_cast<int>(maxLines), 0); i--)
    {
        result.push_back(gContext->logLines.at(i));
    }
    return result;
}

} // namespace Log
