#ifndef UEFI_BLOCK_IO_PROTOCOL_H
#define UEFI_BLOCK_IO_PROTOCOL_H

#include "uefi_base_types.h"

#define EFI_BLOCK_IO_PROTOCOL_GUID \
	{0x964e5b21,0x6459,0x11d2,\
	{0x8e,0x39,0x00,0xa0,0xc9,0x69,0x72,0x3b}}
#define EFI_BLOCK_IO_PROTOCOL_REVISION2 0x00020001
#define EFI_BLOCK_IO_PROTOCOL_REVISION3 ((2<<16) | (31))

typedef UINT64 EFI_LBA;
typedef struct _EFI_BLOCK_IO_PROTOCOL EFI_BLOCK_IO_PROTOCOL;

typedef struct {
	UINT32 MediaId;
	BOOLEAN RemovableMedia;
	BOOLEAN MediaPresent;
	BOOLEAN LogicalPartition;
	BOOLEAN ReadOnly;
	BOOLEAN WriteCaching;
	UINT32 BlockSize;
	UINT32 IoAlign;
	EFI_LBA LastBlock;

	EFI_LBA LowestAlignedLba;
	UINT32 LogicalBlocksPerPhysicalBlocks;
	UINT32 OptimalTransferLengthGranularity;
} EFI_BLOCK_IO_MEDIA;

typedef EFI_STATUS (EFIAPI *EFI_BLOCK_RESET) (
	IN EFI_BLOCK_IO_PROTOCOL *This,
	IN BOOLEAN ExtendedVerification
	);

typedef EFI_STATUS (EFIAPI *EFI_BLOCK_READ) (
	IN EFI_BLOCK_IO_PROTOCOL *This,
	IN UINT32 MediaId,
	IN EFI_LBA LBA,
	IN UINTN BufferSize,
	OUT VOID *Buffer
	);

typedef EFI_STATUS (EFIAPI *EFI_BLOCK_WRITE) (
	IN EFI_BLOCK_IO_PROTOCOL *This,
	IN UINT32 MediaId,
	IN EFI_LBA LBA,
	IN UINTN BufferSize,
	IN VOID *Buffer
	);

typedef EFI_STATUS
(EFIAPI *EFI_BLOCK_FLUSH) (
	IN EFI_BLOCK_IO_PROTOCOL *This
	);

struct _EFI_BLOCK_IO_PROTOCOL {
	UINT64 Revision;
	EFI_BLOCK_IO_MEDIA *Media;
	EFI_BLOCK_RESET Reset;
	EFI_BLOCK_READ ReadBlocks;
	EFI_BLOCK_WRITE WriteBlocks;
	EFI_BLOCK_FLUSH FlushBlocks;
};

#endif /* UEFI_BLOCK_IO_PROTOCOL_H */
