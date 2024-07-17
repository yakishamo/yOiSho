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
} GUID;
typedef GUID EFI_GUID;

typedef UINTN  EFI_STATUS;
typedef VOID*  EFI_HANDLE;
typedef VOID*  EFI_EVENT;
typedef UINT64 EFI_LBA;
typedef UINTN  EFI_TPL;
typedef UINT32 EFI_MAC_ADDRESS;
typedef UINT64 EFI_IPv4_ADDRESS;

#define IN
#define OUT
#define OPTIONAL
#define CONST const
#define EFIAPI


#endif /* UEFI_H */
