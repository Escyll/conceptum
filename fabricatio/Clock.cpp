#include "Clock.h"

Clock::Clock()
{
    m_start = std::chrono::steady_clock::now();
    m_previousDeltaPoint = m_start;
}

float Clock::deltaSeconds()
{
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<float> delta = now - m_previousDeltaPoint;
    m_previousDeltaPoint = now;
    return delta.count();
}

//float Clock::elapsedSeconds()
//{
//    std::chrono::duration<float> elapsed = std::chrono::steady_clock::now() - m_start;
//    return elapsed.count();
//}
