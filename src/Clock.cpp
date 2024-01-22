#include "Clock.h"

Clock::Clock()
{
    m_start = std::chrono::high_resolution_clock::now();
}

float Clock::elapsedSeconds()
{
    std::chrono::duration<float> elapsed = std::chrono::high_resolution_clock::now() - m_start;
    return elapsed.count();
}