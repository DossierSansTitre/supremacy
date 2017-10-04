#ifndef UTIL_PNG_H
#define UTIL_PNG_H

#include <cstdint>

class MemoryFile;
char* load_png(MemoryFile& file, uint32_t* width, uint32_t* height);

#endif
