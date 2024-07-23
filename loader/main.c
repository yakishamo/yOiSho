#include "uefi.h"

EFI_BOOTSERVICES gBS;

EFI_STATUS EFIAPI efi_main(void *ImageHandle __attribute((unused)),
	EFI_SYSTEM_TABLE *SystemTable)
{
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
	SystemTable->ConOut->OutputString(SystemTable->ConOut,
			L"Hello UEFI!\n");


	while(1);
}
