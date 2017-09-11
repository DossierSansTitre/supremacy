#include <noise.h>
#include <math/linear.h>
#include <cmath>
#include <types.h>
#include <log.h>
#include <cstdlib>

constexpr const static float pi = 3.14159265f;

float fade(float t)
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}

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

uint32_t noise_hash(uint32_t a)
{
    a = (a ^ 61) ^ (a >> 16);
    a = a + (a << 3);
    a = a ^ (a >> 4);
    a = a * 0x27d4eb2d;
    a = a ^ (a >> 15);
    return a;
}

uint32_t noise_uniform_hash(uint32_t seed, uint32_t value)
{
    return noise_hash(seed ^ noise_hash(value));
}

uint32_t noise_uniform_hash_2d(uint32_t seed, int32_t x, int32_t y)
{
    uint32_t result;

    result = noise_hash(x + y * 10000 ^ seed);
    return result;
}

float noise_clamped_2d(uint32_t seed, int32_t x, int32_t y)
{
    return (noise_uniform_hash_2d(seed, x, y) % 65536) / 65536.f;
}

static Vector2f gradient_2d(uint32_t seed, int32_t x, int32_t y)
{
    float f;
    Vector2f v;

    f = noise_clamped_2d(seed, x, y) * pi * 2;
    v.x = std::sin(f);
    v.y = std::cos(f);
    return v;
}

float dot_gradient(u32 seed, i32 ix, i32 iy, float x, float y)
{
    float dx = x - ix;
    float dy = y - iy;

    auto v = gradient_2d(seed, ix, iy);
    return v.x * dx + v.y * dy;
}

float perlin(uint32_t seed, float x, float y)
{
    i32 x0 = i32(x);
    i32 x1 = x0 + 1;
    i32 y0 = i32(y);
    i32 y1 = y0 + 1;

    float dx = x - (float)x0;
    float dy = y - (float)y0;

    float t = fade(dx);
    float u = fade(dy);

    float n0, n1, ix0, ix1, value;
    n0 = dot_gradient(seed, x0, y0, x, y);
    n1 = dot_gradient(seed, x1, y0, x, y);
    ix0 = lerp(n0, n1, t);
    n0 = dot_gradient(seed, x0, y1, x, y);
    n1 = dot_gradient(seed, x1, y1, x, y);
    ix1 = lerp(n0, n1, t);
    value = lerp(ix0, ix1, u);

    return value * 1.414f;
}

float noise_fractal_octave_2d(uint32_t seed, uint32_t x, uint32_t y, float persistance, int octaves)
{
    float amplitude;
    float acc;
    float max;

    amplitude = 1.f;
    acc = 0.f;
    max = 0.f;

    for (int i = 0; i < octaves; ++i)
    {
        acc += perlin(seed + i, float(x) / (1 << i), float(y) / (1 << i)) * amplitude;
        max += amplitude;
        amplitude *= persistance;
    }

    return acc / max;
}
