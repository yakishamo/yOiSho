#ifndef MEMORY_H
#define MEMORY_H

#include "../common/memory_map.h"

EFI_MEMORY_DESCRIPTOR* GetUefiMemDesc(UefiMemoryMap *mmap, int i);
void InitializeMemoryMap(UefiMemoryMap *u_mmap);
uint64_t AllocatePage(uint64_t page_size);

#endif /* MEMORY_H */
