#include <noise.h>

float lerp(float a, float b, float alpha)
{
    return (a * (1.f - alpha) + b * alpha);
}

float bilinear(float v00, float v10, float v01, float v11, float dx, float dy)
{
    float y0;
    float y1;

    y0 = lerp(v00, v10, dx);
    y1 = lerp(v01, v11, dx);

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

float noise_fractal_2d(uint32_t seed, float x, float y)
{
    int ix;
    int iy;
    float dx;
    float dy;
    float v[4];

    ix = (int)x;
    iy = (int)y;

    dx = x - ix;
    dy = y - iy;

    v[0] = noise_clamped_2d(seed, ix, iy);
    v[1] = noise_clamped_2d(seed, ix + 1, iy);
    v[2] = noise_clamped_2d(seed, ix, iy + 1);
    v[3] = noise_clamped_2d(seed, ix + 1, iy + 1);

    return bilinear(v[0], v[1], v[2], v[3], dx, dy);
}

float noise_fractal_octave_2d(uint32_t seed, uint32_t x, uint32_t y, float persistance, int octaves)
{
    float amplitude;
    float acc;
    float max;
    float frequency;

    frequency = 1.f;
    amplitude = 1.f;
    acc = 0.f;
    max = 0.f;

    for (int i = 0; i < octaves; ++i)
    {
        acc += noise_fractal_2d(seed, x / frequency, y / frequency) * amplitude;
        max += amplitude;
        amplitude *= persistance;
        frequency *= 2;
    }

    return acc / max;
}
