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

#endif
