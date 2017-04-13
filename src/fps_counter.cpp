#include <cmath>
#include <fps_counter.h>

FpsCounter::FpsCounter()
: _started(false)
{

}

void FpsCounter::update()
{
    if (!_started)
    {
        _last_time = Clock::now();
        _started = true;
        return;
    }
    TimePoint tp = Clock::now();
    Duration delta = (tp - _last_time);
    _last_time = tp;
    uint64_t d = delta.count();
    _samples.push_back(d);
    if (_samples.size() > sample_count)
        _samples.pop_front();
}

uint32_t FpsCounter::get() const
{
    uint64_t acc = 0;

    if (_samples.size() == 0)
        return 0;
    for (size_t i = 0; i < _samples.size(); ++i)
        acc += _samples[i];
    acc /= _samples.size();
    return std::round(1000000000.0 / double(acc));
}
