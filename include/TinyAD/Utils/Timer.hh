/*
 * This file is part of TinyAD and released under the MIT license.
 * Author: Patrick Schmidt
 */
#pragma once

#include <chrono>
#include <atomic>
#include <TinyAD/Utils/Out.hh>

namespace TinyAD
{

struct Timer
{
    using Clock = std::chrono::high_resolution_clock;

    Timer(const std::string& _name, const bool _silent = false) :
        name(_name),
        silent(_silent),
        running(true),
        start_point(Clock::now()),
        duration(0.0)
    { }

    ~Timer()
    {
        if (!silent)
        {
            stop();
            TINYAD_INFO(TINYAD_ANSI_FG_WHITE
                        << "[TIMER] " << name << " took "
                        << seconds()
                        << "s.");
        }
    }

    void start()
    {
        running = true;
        start_point = Clock::now();
    }

    void stop()
    {
        if (running)
            update_duration();

        running = false;
    }

    double seconds()
    {
        if (running)
            return (duration + Clock::now() - start_point).count();
        else
            return duration.count();
    }

private:

    void update_duration()
    {
        std::atomic_thread_fence(std::memory_order_relaxed);
        duration += Clock::now() - start_point;
        std::atomic_thread_fence(std::memory_order_relaxed);
    }

    const std::string name;
    const bool silent;
    bool running;
    typename Clock::time_point start_point;
    std::chrono::duration<double> duration;
};

}
