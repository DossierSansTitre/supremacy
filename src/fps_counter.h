#ifndef FPS_COUNTER_H
#define FPS_COUNTER_H

#include <deque>
#include <chrono>
#include <ratio>
#include <cstdint>
#include <non_copyable.h>

class FpsCounter : private NonCopyable
{
public:
    FpsCounter();
    void        update();
    uint32_t    get() const;

private:
    using Duration = std::chrono::duration<uint64_t, std::nano>;
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock, Duration>;

    std::deque<uint64_t>    _samples;
    TimePoint               _last_time;
    bool                    _started;
};

#endif
