#ifndef MATH_BITWISE_H
#define MATH_BITWISE_H

#include <types.h>

template <typename T>
inline T rotl(T value, int k)
{
    return (value << k) | (value >> (sizeof(T) * 8 - k));
}

template <typename T>
inline T rotr(T value, int k)
{
    return (value << (sizeof(T) * 8 - k)) | (value >> k);
}

inline u64  rotl64(u64 value, int k) { return rotl(value, k); }
inline u32  rotl32(u32 value, int k) { return rotl(value, k); }
inline u16  rotl16(u16 value, int k) { return rotl(value, k); }
inline u8   rotl8(u8 value, int k) { return rotl(value, k); }

inline u64  rotr64(u64 value, int k) { return rotr(value, k); }
inline u32  rotr32(u32 value, int k) { return rotr(value, k); }
inline u16  rotr16(u16 value, int k) { return rotr(value, k); }
inline u8   rotr8(u8 value, int k) { return rotr(value, k); }

#endif
