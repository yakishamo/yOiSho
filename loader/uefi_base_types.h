#ifndef UEFI_BASE_TYPES_H
#define UEFI_BASE_TYPES_H

#include "types64.h"

typedef uint16_t  BOOLEAN;
typedef intptr_t  INTN;
typedef uintptr_t UINTN;

typedef int8_t    INT8;
typedef uint8_t   UINT8;
typedef int16_t   INT16;
typedef uint16_t  UINT16;
typedef int32_t   INT32;
typedef uint32_t  UINT32;
typedef int64_t   INT64;
typedef uint64_t  UINT64;
// How to define INT128 and UINT128 ?

typedef uint8_t   CHAR8;
typedef uint16_t  CHAR16;
typedef void      VOID;

typedef struct {
	UINT32 data1;
	UINT16 data2;
	UINT16 data3;
	UINT8  data4[8];
} __attribute__((packed)) GUID;
typedef GUID EFI_GUID;

typedef UINTN  EFI_STATUS;
typedef VOID*  EFI_HANDLE;
typedef VOID*  EFI_EVENT;
typedef UINT64 EFI_LBA;
typedef UINTN  EFI_TPL;
typedef UINT32 EFI_MAC_ADDRESS;
typedef UINT64 EFI_IPv4_ADDRESS;

typedef UINT64 EFI_PHYSICAL_ADDRESS;
typedef UINT64 EFI_VIRTUAL_ADDRESS;

typedef struct {
	UINT16 Year;
	UINT8  Month;
	UINT8  Day;
	UINT8  Minute;
	UINT8  Second;
	UINT8  Pad1;
	UINT32 Nanosecond;
	INT16  Timezone;
	UINT8  Daylight;
	UINT8  Pad2;
} EFI_TIME;

#define IN
#define OUT
#define OPTIONAL
#define CONST const
#define EFIAPI

// EFI_TABLE_HEADER
typedef struct {
	UINT64 Signature;
	UINT32 Revision;
	UINT32 HeaderSize;
	UINT32 CRC32;
	UINT32 Reserved;
} __attribute__((packed)) EFI_TABLE_HEADER;

// EFI_STATUS
#define EFI_SUCCESS 0
#define EFI_LOAD_ERROR 1
#define INVALID_PARAMETER 2
#define EFI_BUFFER_TOO_SMALL 5

#endif /* UEFI_H */
