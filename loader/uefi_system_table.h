#ifndef UEFI_SYSTEM_TABLE
#define UEFI_SYSTEM_TABLE

#include "uefi_base_types.h"
#include "uefi_bootservices.h"

// EFI_TABLE_HEADER
typedef struct {
	UINT64 Signature;
	UINT32 Revision;
	UINT32 HeaderSize;
	UINT32 CRC32;
	UINT32 Reserved;
} EFI_TABLE_HEADER;

// EFI_SIMPLE_TEXT_INPUT_PROTOCOL
typedef struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

typedef EFI_STATUS (EFIAPI *EFI_INPUT_RESET) (
	IN EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This,
	IN BOOLEAN                         ExtendedVerificaiton
	);

typedef struct {
	UINT16 ScanCode;
	CHAR16 UnicodeChar;
} EFI_INPUT_KEY;

typedef EFI_STATUS (EFIAPI *EFI_INPUT_READ_KEY) (
	IN EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This,
	OUT EFI_INPUT_KEY                 *Key
	);

typedef VOID *EFI_EVENT;

struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL {
	EFI_INPUT_RESET    Reset;
	EFI_INPUT_READ_KEY ReadKeyStroke;
	EFI_EVENT          WaitForKey;
};

// EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL
typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef EFI_STATUS (EFIAPI *EFI_TEXT_RESET) (
	IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
	IN BOOLEAN                          ExtendedVerification
	);

typedef EFI_STATUS (EFIAPI *EFI_TEXT_STRING) (
	IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
	IN CHAR16                          *String
	);

typedef EFI_STATUS (EFIAPI *EFI_TEXT_TEST_STRING) (
	IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
	IN CHAR16                          *String
	);

typedef EFI_STATUS (EFIAPI *EFI_TEXT_QUERY_MODE) (
	IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
	IN UINTN                            ModeNumber,
	OUT UINTN                          *Columns,
	OUT UINTN                          *Rows
	);

typedef EFI_STATUS (* EFIAPI EFI_TEXT_SET_MODE) (
	IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
	IN UINTN                            ModeNumber
	);

typedef EFI_STATUS (EFIAPI *EFI_TEXT_SET_ATTRIBUTE) (
	IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
	IN UINTN                            Attribute
	);

typedef EFI_STATUS (EFIAPI *EFI_TEXT_CLEAR_SCREEN) (
	IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This
	);

typedef EFI_STATUS (EFIAPI *EFI_TEXT_SET_CURSOR_POSITION) (
	IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
	IN UINTN                            Column,
	IN UINTN                            Row
	);

typedef EFI_STATUS (EFIAPI *EFI_TEXT_ENABLE_CURSOR) (
	IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
	IN BOOLEAN                           Visible
	);

typedef struct {
	INT32   MaxMode;
	INT32   Mode;
	INT32   Attribute;
	INT32   CursorColumn;
	INT32   CursorRow;
	BOOLEAN   CursorVisible;
} SIMPLE_TEXT_OUTPUT_MODE;

struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
	EFI_TEXT_RESET               Reset;
	EFI_TEXT_STRING              OutputString;
	EFI_TEXT_TEST_STRING         TestString;
	EFI_TEXT_QUERY_MODE          QueryMode;
	EFI_TEXT_SET_MODE            SetMode;
	EFI_TEXT_SET_ATTRIBUTE       SetAttribute;
	EFI_TEXT_CLEAR_SCREEN        ClearScreen;
	EFI_TEXT_SET_CURSOR_POSITION SetCursorPosition;
	EFI_TEXT_ENABLE_CURSOR       EnableCursor;
	SIMPLE_TEXT_OUTPUT_MODE      *Mode;
};

// EFI_RUNTIME_SERVIECES(unuse)
typedef struct {
	EFI_TABLE_HEADER Hdr;
	UINT8            buf[sizeof(UINTN)*14];
}EFI_RUNTIME_SERVICES; 

// EFI_BOOT_SERVICES
typedef struct {
	EFI_TABLE_HEADER Hdr;
	UINT8            buf[sizeof(UINTN)*44];
}EFI_BOOT_SERVICES ;

typedef struct {
	EFI_GUID VendorGuid;
	VOID    *VendorTable;
}EFI_CONFIGURATION_TABLE;

// EFI_SYSTEM_TABLE
#define EFI_SYSTEM_TABLE_SIGNATURE 0x5453595320494249
typedef struct {
	EFI_TABLE_HEADER                 Hdr;
	CHAR16                          *FirmwareVendor;
	UINT32                           FirmwareRevision;
	EFI_HANDLE                       ConsoleInHandle;
	EFI_SIMPLE_TEXT_INPUT_PROTOCOL  *ConIn;
	EFI_HANDLE                       ConsoleOutHandle;
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
	EFI_HANDLE                       ConsoleErrorHandle;
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConErr;
	EFI_RUNTIME_SERVICES            *RuntimeServices;
	EFI_BOOT_SERVICES               *BootServices;
	UINTN                            NumberOfTableEntries;
	EFI_CONFIGURATION_TABLE         *ConfigurationTable;
} EFI_SYSTEM_TABLE;

#endif /* UEFI_SYSTEM_TABLE */
