#include "uefi.h"
#include "uefi_loaded_image_protocol.h"
#include "uefi_simple_file_system_protocol.h"
#include "uefi_file_protocol.h"
#include "elf.h"

EFI_BOOT_SERVICES *gBS;
EFI_SYSTEM_TABLE *gST;

void hlt() {
	while(1) asm("hlt");
}

VOID Print(CHAR16 *str) {
	gST->ConOut->OutputString(gST->ConOut, str);
	return;
}

UINTN pow(UINTN a, UINTN b) {
	UINTN ret = 1;
	for(UINTN i = 0; i < b; i++) {
		ret *= a;
	}
	return ret;
}

VOID Print_int(UINTN a, unsigned int radix) {
	CHAR16 str[25];
	CHAR16 *p = str;
	unsigned int v = a;
	int n = 1;
	while(v >= radix) {
		v/=radix;
		n++;
	}
	p = str + n;
	v = a;
	*p = 0;
	do {
		p--;
		*p = v % radix + (CHAR16)'0';
		if(*p > (CHAR16)'9') {
			*p = v % radix - 10 + 'A';
		}
		v /= radix;
	} while(p != str);
	Print(str);
}

EFI_STATUS EFIAPI efi_main(void *image_handle __attribute((unused)),
	EFI_SYSTEM_TABLE *system_table)
{
	EFI_STATUS status;
	gST = system_table;
	gBS = gST->BootServices;

	gST->ConOut->ClearScreen(gST->ConOut);
	gST->ConOut->OutputString(gST->ConOut,
			L"Hello, UEFI!\r\n");

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

	// allocate tmp buffer for kernel
	UINTN kernel_file_size = (UINTN)kernel_info->FileSize;
	CHAR8 *kernel_tmp_buf;
	status = gBS->AllocatePool(
			EfiLoaderData,
			kernel_file_size,
			(VOID**)&kernel_tmp_buf
			);
	if(status != EFI_SUCCESS) {
		Print(L"failed to allocate pool for kernel_tmp_buf\r\n");
		hlt();
	}
	Print(L"allocate pool for kernel_tmp_buf\r\n");

	// read kernel
	status = kernel_file->Read(
			kernel_file,
			&kernel_file_size,
			kernel_tmp_buf
			);
	if(status != EFI_SUCCESS) {
		Print(L"failed to read kernel.elf\r\n");
		hlt();
	}
	Print(L"read kernel.elf\r\n");
	if(kernel_tmp_buf[0] != 0x7f ||
			kernel_tmp_buf[1] != 'E' || 
			kernel_tmp_buf[2] != 'L' ||
			kernel_tmp_buf[3] != 'F') {
		Print(L"read kernel is not elf format.\r\n");
		hlt();
	}
	Print(L"this is elf format\r\n");

	/*
	// allocate kernel buffer
	status = gBS->AllocatePages(
			EfiLoaderData,
	*/

	// read kernel elf header
	Elf64_Ehdr *ehdr = (Elf64_Ehdr*)kernel_tmp_buf;
	typedef void (*kernel_ent_t)(void);
	kernel_ent_t kernel_ent;
	kernel_ent = (kernel_ent_t)((UINTN)ehdr->e_entry + (UINTN)kernel_tmp_buf - 0x100000);

	Print(L"0x");
	Print_int((UINTN)kernel_ent, 16);

	kernel_ent();

	hlt();
	return EFI_SUCCESS;
}
