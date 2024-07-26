#include "uefi.h"
#include "uefi_loaded_image_protocol.h"
#include "uefi_simple_file_system_protocol.h"
#include "uefi_file_protocol.h"

EFI_BOOT_SERVICES *gBS;
EFI_SYSTEM_TABLE *gST;

void hlt() {
	while(1) asm("hlt");
}

VOID Print(CHAR16 *str) {
	gST->ConOut->OutputString(gST->ConOut, str);
	return;
}

EFI_STATUS EFIAPI efi_main(void *image_handle __attribute((unused)),
	EFI_SYSTEM_TABLE *system_table)
{
	EFI_STATUS status;
	gST = system_table;
	gBS = gST->BootServices;

	gST->ConOut->ClearScreen(gST->ConOut);
	gST->ConOut->OutputString(gST->ConOut,
			L"Hello UEFI!\r\n");

	// open loaded image protocol
	EFI_GUID loaded_image_guid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
	EFI_LOADED_IMAGE_PROTOCOL *loaded_image;
	status = gBS->OpenProtocol(
			image_handle,
			&loaded_image_guid,
			(VOID**)&loaded_image,
			image_handle,
			NULL,
			EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
	if(status != EFI_SUCCESS) {
		Print(L"failed to open EFI_LOADED_IMAGE_PROTOCOL\r\n");
		hlt();
	}
	Print(L"open EFI_LOADED_IMAGE_PROTOCOL\r\n");

	// open simple file system protocol
	EFI_GUID simple_file_system_protocol_guid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fs;
	status = gBS->OpenProtocol(
			loaded_image->DeviceHandle,
			&simple_file_system_protocol_guid,
			(VOID**)&fs,
			image_handle,
			NULL,
			EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
	if(status != EFI_SUCCESS) {
		Print(L"failed to open EFI_SIMPLE_FILE_SYSTEM_PROTOCOL\r\n");
		hlt();
	}
	Print(L"open EFI_SIMPLE_FILE_SYSTEM_PROTOCOL\r\n");

	// open root
	EFI_FILE_PROTOCOL *root_dir;
	status = fs->OpenVolume(fs, &root_dir);
	if(status != EFI_SUCCESS) {
		Print(L"failed to open root.\r\n");
		hlt();
	}
	Print(L"open root\r\n");

	// open kernel.elf
	EFI_FILE_PROTOCOL *kernel_file;
	status = root_dir->Open(
		root_dir, &kernel_file, L"\\kernel.elf",
		EFI_FILE_MODE_READ, 0);
	if( status != EFI_SUCCESS) {
		Print(L"failed to open kernel.elf\r\n");
		hlt();
	}
	Print(L"open kernel.elf\r\n");

	// get kernel info
	CHAR8 kernel_info_buf[sizeof(EFI_FILE_INFO)+100];
	UINTN kernel_info_size = (UINTN)sizeof(kernel_info_buf);
	EFI_FILE_INFO *kernel_info = (EFI_FILE_INFO*)kernel_info_buf;
	EFI_GUID file_info_guid = EFI_FILE_INFO_ID;
	status = kernel_file->GetInfo(
			kernel_file,
			&file_info_guid,
			&kernel_info_size,
			kernel_info);
	if(status != EFI_SUCCESS) {
		Print(L"failed to get kernel info\r\n");
		hlt();
	}
	Print(kernel_info->FileName);

	// load kernel to temp memory
	UINT64 kernel_file_size = kernel_info->FileSize;

	hlt();
	return EFI_SUCCESS;
}

