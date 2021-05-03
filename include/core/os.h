#ifndef CORE_OS_H
#define CORE_OS_H

#if defined(WIN32)
# define OS_WINDOWS 1
#elif defined(__APPLE__)
# define OS_MAC 1
#elif defined(__linux__)
# define OS_LINUX 1
#else
# error "Unknown Operating System"
#endif

#if defined(OS_MAC) || defined(OS_LINUX)
# define OS_UNIX 1
#endif

#if defined(__LP64__) || defined(__LLP64__) || defined(_M_X64) || defined(__amd64__)
# define OS_64 1
# define OS_BITS 64
#else
# define OS_32 1
# define OS_BITS 32
#endif

#define OS_BITS_BYTES   (OS_BITS / 8)

#endif
