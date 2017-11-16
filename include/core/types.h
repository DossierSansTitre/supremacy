#ifndef INCLUDED_CORE_TYPES_H
#define INCLUDED_CORE_TYPES_H

#include <core/os.h>
#include <cstdint>

using i8    = int8_t;
using i16   = int16_t;
using i32   = int32_t;
using i64   = int64_t;
using u8    = uint8_t;
using u16   = uint16_t;
using u32   = uint32_t;
using u64   = uint64_t;

#if OS_64
using iword = int64_t;
using uword = uint64_t;
#else
using iword = int32_t;
using uword = uint32_t;
#endif

#endif
