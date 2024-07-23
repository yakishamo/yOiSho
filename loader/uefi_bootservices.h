#ifndef UEFI_BOOTSERVICES
#define UEFI_BOOTSERVICES

#include "uefi_base_types.h"
#include "uefi_memory.h"
#include "uefi_memory_map.h"

typedef enum {
	AllocateAnyPages,
	AllocateMaxAddress,
	AllocateAddres,
	MaxAllocateType
} EFI_ALLOCATE_TYPE;

typedef EFI_STATUS (EFIAPI *EFI_ALLOCATE_PAGES) (
	IN EFI_ALLOCATE_TYPE         Type,
	IN EFI_MEMORY_TYPE           MemoryType,
	IN UINTN                     Pages,
	IN OUT EFI_PHYSICAL_ADDRESS *Memory
	);

typedef EFI_STATUS (EFIAPI *EFI_FREE_PAGES) (
	IN EFI_PHYSICAL_ADDRESS Memory,
	IN UINTN                Pages
	);

typedef EFI_STATUS (EFIAPI *EFI_GET_MEMORY_MAP) (
	IN OUT UINTN              *MemoryMapSize,
	OUT EFI_MEMORY_DESCRIPTOR *MemoryMap,
	OUT UINTN                 *MapKey,
	OUT UINTN                 *DescriptorSize,
	OUT UINT32                *DescriptorVersion
	);

typedef EFI_STATUS (EFIAPI *EFI_ALLOCATE_POOL) (
	IN EFI_MEMORY_TYPE PoolType,
	IN UINTN           Size,
	OUT VOID         **Buffer
	);

typedef EFI_STATUS (EFIAPI *EFI_FREE_POOL) (
	IN VOID *Buffer
	);

#endif /* UEFI_BOOTSERVICES */
