#ifndef UEFI_MEMORY_MAP
#define UEFI_MEMORY_MAP

#include "uefi_base_types.h"
#include "uefi_memory.h"

typedef struct {
	UINT32               Type; //EFI_MEMORY_TYPE
	EFI_PHYSICAL_ADDRESS PhysicalStart;
	EFI_VIRTUAL_ADDRESS  VirtualStart;
	UINT64               NumberOfPages;
	UINT64               Attribute;
} EFI_MEMORY_DESCRIPTOR;

#endif /* UEFI_MEMORY_MAP */
