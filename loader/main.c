#include "../uefi/uefi.h"
#include "../uefi/uefi_loaded_image_protocol.h"
#include "../uefi/uefi_simple_file_system_protocol.h"
#include "../uefi/uefi_file_protocol.h"
#include "../uefi/uefi_graphics_output_protocol.h"
#include "../common/elf.h"
#include "../common/frame_info.h"
#include "../common/memory_map.h"

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

	// read kernel elf header
	Elf64_Ehdr *ehdr = (Elf64_Ehdr*)kernel_tmp_buf;
	Elf64_Half ph_ent_size = ehdr->e_phentsize;
	Elf64_Half ph_num = ehdr->e_phnum;
	UINTN kernel_start_addr = 0xffffffffffff;
	UINTN kernel_end_addr = 0x0;

	// calc kernel mem size and allocate kernel code memory
	for(int i = 0; i < ph_num; i++) {
		Elf64_Phdr *phdr = (Elf64_Phdr*)((UINTN)kernel_tmp_buf + ehdr->e_phoff + i*ph_ent_size);
		if(phdr->p_type != PT_LOAD)
			continue;
	
		if(phdr->p_vaddr < kernel_start_addr) 
			kernel_start_addr = (UINTN)phdr->p_vaddr;
		if(kernel_end_addr < (phdr->p_vaddr + phdr->p_memsz))
			kernel_end_addr = phdr->p_vaddr + phdr->p_memsz;
	}
	UINTN page_num = (kernel_end_addr - kernel_start_addr) / 0x1000 + 1;
	status = gBS->AllocatePages(
			AllocateAddress,
			EfiLoaderData,
			page_num,
			&kernel_start_addr
			);
	if(status != EFI_SUCCESS) {
		Print(L"failed to allocate kernel buffer\r\n");
		Print(L"kernel_start_addr : 0x");
		Print_int(kernel_start_addr, 16);
		Print(L"\r\n");
		hlt();
	}
	Print(L"allocate kernel buffer successfully\r\n");
	Print(L"kernel_start_addr : 0x");
	Print_int(kernel_start_addr, 16);
	Print(L"\r\nkernel_end_addr : 0x");
	Print_int(kernel_end_addr, 16);
	Print(L"\r\n");

	// copy kernel
	gBS->SetMem(
			(VOID*)kernel_start_addr,
			kernel_end_addr - kernel_start_addr + 1,
			0
			);
	for(int i = 0; i < ph_num; i++) {
		Elf64_Phdr *phdr = (Elf64_Phdr*)((UINTN)kernel_tmp_buf + ehdr->e_phoff+i*ph_ent_size);
		if(phdr->p_type != PT_LOAD)
			continue;
		gBS->CopyMem(
				(VOID*)phdr->p_vaddr,
				(VOID*)((UINTN)kernel_tmp_buf + phdr->p_offset),
				phdr->p_filesz
				);
		Print(L"\r\ncopy\r\ndst : 0x");
		Print_int((UINTN)phdr->p_vaddr, 16);
		Print(L"\r\nsrc : 0x");
		Print_int((UINTN)kernel_tmp_buf + phdr->p_offset, 16);
		Print(L"\r\nlen : 0x");
		Print_int((UINTN)phdr->p_filesz, 16);
		Print(L"\r\n");
	}

	EFI_GUID gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GRAPHICS_OUTPUT_PROTOCOL *gop = NULL;
	status = gBS->LocateProtocol(
			&gop_guid,
			NULL,
			(VOID**)&gop
			);
	if(status != EFI_SUCCESS) {
		Print(L"Locate gop failed.\r\n");
		hlt();
	}

	// prepare frame info
	FrameInfo *fi;
	status = gBS->AllocatePool(
			EfiLoaderData,
			sizeof(FrameInfo),
			(VOID**)&fi
			);
	if(status != EFI_SUCCESS) {
		Print(L"failed to allocate pool for frame info\r\n");
		hlt();
	}
	fi->frame_base = (unsigned char *)gop->Mode->FrameBufferBase;
	fi->frame_size = gop->Mode->FrameBufferSize;
	fi->format = (PixelFormat)gop->Mode->Info->PixelFormat;
	fi->mask.red = gop->Mode->Info->PixelInformation.RedMask;
	fi->mask.green = gop->Mode->Info->PixelInformation.GreenMask;
	fi->mask.blue = gop->Mode->Info->PixelInformation.BlueMask;
	fi->mask.reserved = gop->Mode->Info->PixelInformation.ReservedMask;
	fi->horizontal_resolution = gop->Mode->Info->HorizontalResolution;
	fi->vertical_resolution = gop->Mode->Info->VerticalResolution;
	fi->pixel_per_scanline = gop->Mode->Info->PixelsPerScanLine;

	Print(L"kernel_entry : 0x");
	Print_int((UINTN)ehdr->e_entry,16);
	Print(L"\r\n");

	// exit bootservice
	UefiMemoryMap *memory_map = NULL;
	status = gBS->AllocatePool(
			EfiLoaderData,
			sizeof(UefiMemoryMap),
			(VOID**)&memory_map
			);
	if(status != EFI_SUCCESS) {
		Print(L"failed to allocate pool for memory map\r\n");
		hlt();
	}
	memory_map->desc = NULL;
	memory_map->desc_size = 0;
	memory_map->desc_ver = 0;
	memory_map->map_size = 4096*4;
	memory_map->map_key = 0;
	status = gBS->AllocatePool(
			EfiLoaderData,
			memory_map->map_size,
			(VOID**)&memory_map->desc
			);
	if(status != EFI_SUCCESS) {
		Print(L"failed to allocate pool for memory descriptor\r\n");
		hlt();
	}

	status = gBS->GetMemoryMap(
			&memory_map->map_size,
			memory_map->desc,
			&memory_map->map_key,
			&memory_map->desc_size,
			&memory_map->desc_ver
			);
	if(status != EFI_SUCCESS) {
		Print(L"failed to get memory map\r\nstatus = 0x");
		Print_int((UINTN)status,16);
		hlt();
	}

	status = gBS->ExitBootServices(image_handle, memory_map->map_key);
	if(status != EFI_SUCCESS) {
		Print(L"faield to exit bootservices\r\n");
		hlt();
	}

	typedef void (*kernel_main_t) (FrameInfo *frame_info, UefiMemoryMap *memory_map);
	kernel_main_t kernel_main = (kernel_main_t)ehdr->e_entry;
	kernel_main(fi, memory_map);

	hlt();
	return EFI_SUCCESS;
}
