#ifndef CLOCK_H
#define CLOCK_H

#pragma once

#include <chrono>

class Clock
{
public:
    Clock();

    //float elapsedSeconds();
    float deltaSeconds();

private:
    std::chrono::steady_clock::time_point m_start;
    std::chrono::steady_clock::time_point m_previousDeltaPoint;
};

#endif