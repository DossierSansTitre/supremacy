#ifndef NOISE_H
#define NOISE_H

#include <cstdint>

float       lerp(float a, float b, float alpha);
uint32_t    noise_hash(uint32_t value);
uint32_t    noise_uniform_hash(uint32_t seed, uint32_t value);
uint32_t    noise_uniform_hash_2d(uint32_t seed, int32_t x, int32_t y);
float       noise_clamped_2d(uint32_t seed, int32_t x, int32_t y);
float       noise_fractal_2d(uint32_t seed, int32_t x, int32_t y);
float       noise_fractal_octave_2d(uint32_t seed, uint32_t x, uint32_t y, float persistance, int octaves);

#endif
