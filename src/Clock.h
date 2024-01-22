#ifndef CLOCK_H
#define CLOCK_H

#pragma once

#include <chrono>

class Clock
{
public:
    Clock();

    float elapsedSeconds();

private:
    std::chrono::high_resolution_clock::time_point m_start;
};

#endif