#ifndef LOGGER_H
#define LOGGER_H

#include <filesystem>
#include <string>
#include <vector>

namespace Log {

struct LoggerContext;
class Log
{
public:
    Log& operator<<(const char* value);
    Log& operator<<(const std::string& value);
    Log& operator<<(const std::filesystem::path& value);
    template<typename T>
    Log& operator<<(T value);
    ~Log();
};

void setContext(LoggerContext* context);
LoggerContext* context();
LoggerContext* createContext();
void destroyContext(LoggerContext* context);

Log log();
std::vector<std::string> lastNLogLines(unsigned int maxLines);

} // namespace Log

#endif
