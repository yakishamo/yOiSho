#ifndef MEMORY_MAP
#define MEMORY_MAP

#include "uefi_memory_map.h"

typedef struct {
	EFI_MEMORY_DESCRIPTOR *desc;
	UINTN desc_size;
	UINT32 desc_ver;
	UINTN map_size;
	UINTN map_key;
} MemoryMap;

#endif /* MEMORY_MAP */