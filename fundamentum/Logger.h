#ifndef LOGGER_H
#define LOGGER_H

#include <filesystem>
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace Log {

struct End
{};
static End end;
struct LoggerContext;
class Log
{
public:
    Log& operator<<(const char* value);
    Log& operator<<(const std::string& value);
    Log& operator<<(const std::filesystem::path& value);
    Log& operator<<(const glm::mat4& value);
    template<typename T>
    Log& operator<<(T value);
    void operator<<(const End& value);
};

void setContext(LoggerContext* context);
LoggerContext* context();
LoggerContext* createContext();
void destroyContext(LoggerContext* context);

Log log();
std::vector<std::string> lastNLogLines(unsigned int maxLines);

} // namespace Log

#endif
