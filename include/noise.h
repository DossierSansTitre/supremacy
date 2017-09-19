#ifndef NOISE_H
#define NOISE_H

#include <types.h>

float       lerp(float a, float b, float alpha);
uint32_t    noise_hash(uint32_t value);
uint32_t    noise_uniform_hash(uint32_t seed, uint32_t value);
uint32_t    noise_uniform_hash_2d(uint32_t seed, int32_t x, int32_t y);
float       noise_clamped_2d(uint32_t seed, int32_t x, int32_t y);
float       noise_smooth_2d(uint32_t seed, int32_t x, int32_t y, int octave);
float       perlin(uint32_t seed, float x, float y);
float       noise_fractal_octave_2d(uint32_t seed, uint32_t x, uint32_t y, float persistance, int octaves);
float       perlin_octave_corrected(u32 seed, u32 x, u32 y, float persistance, int octaves, float correction);

#endif
