#ifndef UEFI_BOOTSERVICES
#define UEFI_BOOTSERVICES

#include "uefi_base_types.h"
#include "uefi_memory.h"
#include "uefi_memory_map.h"

typedef enum _EFI_ALLOCATE_TYPE {
	AllocateAnyPages,
	AllocateMaxAddress,
	AllocateAddress,
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

typedef EFI_STATUS (EFIAPI *EFI_EXIT) (
		IN EFI_HANDLE ImageHandle,
		IN EFI_STATUS ExitStatus,
		IN UINTN      ExitDataSize,
		IN CHAR16    *ExitData OPTIONAL
		);

typedef EFI_STATUS (EFIAPI *EFI_EXIT_BOOT_SERVICES) (
		IN EFI_HANDLE ImageHandle,
		IN UINTN      MapKey
		);

typedef EFI_STATUS (EFIAPI *EFI_OPEN_PROTOCOL) (
		IN EFI_HANDLE Handle,
		IN EFI_GUID  *Protocol,
		OUT VOID    **Interface OPTIONAL,
		IN EFI_HANDLE AgentHandle,
		IN EFI_HANDLE ControllerHandle,
		IN UINT32     Attributes
		);
// Attributes
#define EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL  0x00000001
#define EFI_OPEN_PROTOCOL_GET_PROTOCOL        0x00000002
#define EFI_OPEN_PROTOCOL_TEST_PROTOCOL       0x00000004
#define EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER 0x00000008
#define EFI_OPEN_PROTOCOL_BY_DRIVER           0x00000010
#define EFI_OPEN_PROTOCOL_EXCLUSIVE           0x00000020

typedef EFI_STATUS (EFIAPI *EFI_CLOSE_PROTOCOL) (
		IN EFI_HANDLE Handle,
		IN EFI_GUID  *Protocol,
		IN EFI_HANDLE AgentHandle,
		IN EFI_HANDLE ControllerHandle
		);

typedef enum {
	AllHandles,
	ByRegisterNotify,
	ByProtocol
} EFI_LOCATE_SEARCH_TYPE;

typedef EFI_STATUS (EFIAPI *EFI_LOCATE_HANDLE_BUFFER) (
		IN EFI_LOCATE_SEARCH_TYPE SearchType,
		IN EFI_GUID              *Protocol OPTIONAL,
		IN VOID                  *SearchKey OPTIONAL,
		OUT UINTN                *NoHandles,
		OUT EFI_HANDLE          **Buffer
		);

typedef EFI_STATUS (EFIAPI *EFI_LOCATE_PROTOCOL) (
		IN EFI_GUID *Protocol,
		IN VOID     *Registration OPTIONAL,
		OUT VOID   **Interface
		);

typedef VOID (EFIAPI *EFI_COPY_MEM) (
		IN VOID *Destination,
		IN VOID *Source,
		IN UINTN Length
		);

typedef VOID (EFIAPI *EFI_SET_MEM) (
		IN VOID *Buffer,
		IN UINTN Size,
		IN UINT8 Value
		);

typedef struct {
	EFI_TABLE_HEADER       Hdr;
	UINTN                  buf1[2];
	EFI_ALLOCATE_PAGES     AllocatePages;
	EFI_FREE_PAGES         FreePages;
	EFI_GET_MEMORY_MAP     GetMemoryMap;
	EFI_ALLOCATE_POOL      AllocatePool;
	EFI_FREE_POOL          FreePool;
	UINTN                  buf2[17];
	EFI_EXIT               Exit;
	UINTN                  buf3[1];
	EFI_EXIT_BOOT_SERVICES ExitBootServices;
	UINTN                  buf4[5];
	EFI_OPEN_PROTOCOL      OpenProtocol;
	EFI_CLOSE_PROTOCOL     CloseProtocol;
	UINTN                  buf5[2];
	EFI_LOCATE_HANDLE_BUFFER LocateHandleBuffer;
	EFI_LOCATE_PROTOCOL    LocateProtocol;
	UINTN                  buf6[2];
	EFI_COPY_MEM           CopyMem;
	EFI_SET_MEM            SetMem;
	UINTN                  buf7[1];
} __attribute__((packed)) EFI_BOOT_SERVICES;

#endif /* UEFI_BOOTSERVICES */
