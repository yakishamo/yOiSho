#ifndef MEMORY_H
#define MEMORY_H

#include "../common/memory_map.h"

EFI_MEMORY_DESCRIPTOR* GetUefiMemDesc(UefiMemoryMap *mmap, int i);
void InitMemoryMap(UefiMemoryMap *u_mmap);

#endif /* MEMORY_H */
