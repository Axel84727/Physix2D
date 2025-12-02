#pragma once

#include <chrono>
#include <cstdint>

struct Accumulator
{
    uint64_t count = 0;
    uint64_t total_us = 0;

    void add(uint64_t us)
    {
        total_us += us;
        ++count;
    }

    double avg_us() const
    {
        if (count == 0)
            return 0.0;
        return double(total_us) / double(count);
    }

    void reset()
    {
        count = 0;
        total_us = 0;
    }
};

struct ScopedTimer
{
    Accumulator &acc;
    std::chrono::high_resolution_clock::time_point start;
    ScopedTimer(Accumulator &a) : acc(a), start(std::chrono::high_resolution_clock::now()) {}
    ~ScopedTimer()
    {
        auto end = std::chrono::high_resolution_clock::now();
        auto us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        acc.add(uint64_t(us));
    }
};
