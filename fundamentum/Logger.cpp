#include <string>
#include <vector>
#include <iostream>
#include <cmath>

#include "Logger.h"

Logger logger;

void Logger::Log(const std::string& text)
{
    logger.m_context->push_back(text);
}

std::vector<std::string> Logger::LastNLogLines(unsigned int maxLines)
{
    std::vector<std::string> result;
    int nLines = logger.m_context->size();
    std::cout << nLines - 1 - maxLines << std::endl;
    for (int i = nLines - 1; i > std::max(nLines - 1 - static_cast<int>(maxLines), 0) ; i--)
    {
        result.push_back(logger.m_context->at(i));
    }
    std::cout << result.size() << std::endl;
    return result;
}

