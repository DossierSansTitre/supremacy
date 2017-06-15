#ifndef ALLOC_MEMORY_H
#define ALLOC_MEMORY_H

namespace Memory
{

constexpr const size_t page_size_bits = 12;
constexpr const size_t large_page_size_bits = 21;
constexpr const size_t huge_page_size_bits = 30;

constexpr const size_t page_size = (1 << page_size_bits);
constexpr const size_t large_page_size = (1 << large_page_size_bits);
constexpr const size_t huge_page_size = (1 << huge_page_size_bits);

void*   alloc_pages(size_t count);
void    free_pages(void* size_t count);

}

#endif
