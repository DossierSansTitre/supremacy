#ifndef MATH_RNG_H
#define MATH_RNG_H

#include <types.h>
#include <math/bitwise.h>

class Rng
{
public:
    Rng()
    : _state()
    {

    }

    const u64* state() const
    {
        return _state;
    }

    void seed(const u64 values[2])
    {
        _state[0] = values[0];
        _state[1] = values[1];
    }

    void seed(Rng& rng)
    {
        do
        {
            _state[0] = rng.rand();
            _state[1] = rng.rand();
        } while (_state[0] == 0 && _state[1] == 0);
    }

    u64 rand()
    {
        u64 s0;
        u64 s1;
        u64 result;

        s0 = _state[0];
        s1 = _state[1];
        result = s0 + s1;
        s1 ^= s0;
        _state[0] = rotl64(s0, 55) ^ s1 ^ (s1 << 14);
        _state[1] = rotl64(s1, 36);

        return result;
    }

private:
    u64     _state[2];
};

#endif
