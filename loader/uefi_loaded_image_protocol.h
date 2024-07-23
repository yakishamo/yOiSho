#ifndef UEFI_LOADED_IMAGE_PROTOCOL
#define UEFI_LOADED_IMAGE_PROTOCOL

#include "uefi_base_types.h"

#define EFI_LOADED_IMAGE_PROTOCOL_GUID \
	{0x5B1B31A1, 0x9562, 0x11d2, \
		{0x8E, 0x3F, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B}}

#define EFI_LOADED_IMAGE_PROTOCOL_REVISION 0x1000

typedef struct {
	UINT32            Revision;
	EFI_HANDLE        ParentHandle;
	EFI_SYSTEM_TABLE *SystemTable;

	EFI_HANDLE        DeviceHandle;
	UINTN             buf[];
} EFI_LOADED_IMAGE_PROTOCOL;

#endif /* UEFI_LOADED_IMAGE_PROTOCOL */
