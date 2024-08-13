#ifndef LOGGER_H
#define LOGGER_H

#include <vector>
#include <string>
#include <memory>

using LoggerContext = std::vector<std::string>;
using SharedLoggerContext = std::shared_ptr<LoggerContext>;

class Logger
{
public:
    static void Log(const std::string& text);
    static std::vector<std::string> LastNLogLines(unsigned int maxLines);
    static SharedLoggerContext LoggerContext();
    static void SetLoggerContext(const SharedLoggerContext& loggerContext);

private:
    SharedLoggerContext m_context = std::make_shared<::LoggerContext>();
};

#endif
