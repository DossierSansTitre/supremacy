#include <noise.h>

float lerp(float a, float b, float alpha)
{
    return (a * (1.f - alpha) + b * alpha);
}

float bilinear(const float* v, float dx, float dy)
{
    float y0;
    float y1;

    y0 = lerp(v[0], v[1], dx);
    y1 = lerp(v[2], v[3], dx);

    return lerp(y0, y1, dy);
}

static uint32_t number_pair(uint32_t a, uint32_t b)
{
    return (a > b) ? (a * a + a + b) : (a + b * b);
}

uint32_t noise_hash(uint32_t value)
{
    value = ((value >> 16) ^ value) * 0x45d9f3b;
    value = ((value >> 16) ^ value) * 0x45d9f3b;
    value = ((value >> 16) ^ value);
    return value;
}

uint32_t noise_uniform_hash(uint32_t seed, uint32_t value)
{
    return noise_hash(seed ^ noise_hash(value));
}

uint32_t noise_uniform_hash_2d(uint32_t seed, int32_t x, int32_t y)
{
    return noise_hash(noise_hash(number_pair(x, y)) ^ seed);
}

float noise_clamped_2d(uint32_t seed, int32_t x, int32_t y)
{
    return noise_uniform_hash_2d(seed, x, y) / 4294967295.f;
}

float noise_smooth_2d(uint32_t seed, int32_t x, int32_t y, int octave)
{
    float v[4];
    float dx;
    float dy;
    int32_t pot;
    int32_t alt_x;
    int32_t alt_y;
    int32_t rem_x;
    int32_t rem_y;

    pot = (1 << octave);
    alt_x = x / pot;
    alt_y = y / pot;
    rem_x = x % pot;
    rem_y = y % pot;
    dx = float(rem_x) / pot;
    dy = float(rem_y) / pot;

    v[0] = noise_clamped_2d(seed, alt_x, alt_y);
    v[1] = noise_clamped_2d(seed, alt_x + 1, alt_y);
    v[2] = noise_clamped_2d(seed, alt_x, alt_y + 1);
    v[3] = noise_clamped_2d(seed, alt_x + 1, alt_y + 1);

    return bilinear(v, dx, dy);
}

float noise_fractal_octave_2d(uint32_t seed, uint32_t x, uint32_t y, float persistance, int octaves)
{
    float amplitude;
    float acc;
    float max;

    amplitude = persistance;
    acc = noise_clamped_2d(seed, x, y);
    max = 1.f;

    for (int i = 1; i < octaves; ++i)
    {
        acc += noise_smooth_2d(seed, x, y, i) * amplitude;
        max += amplitude;
        amplitude *= persistance;
    }

    return acc / max;
}
