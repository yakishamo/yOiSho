#include "../common/memory_map.h"
#include "../uefi/uefi_memory_map.h"
#include "../uefi/uefi_memory.h"
#include "memory.h"
#include "string.h"
#include "frame.h"

#define MEMORY_MAP_SIZE 1024*1024/8

char memory_map[MEMORY_MAP_SIZE];

EFI_MEMORY_DESCRIPTOR* GetUefiMemDesc(UefiMemoryMap *mmap, int i) {
	uintptr_t ret = (uintptr_t)mmap->desc;
	uintptr_t limit = (uintptr_t)mmap->desc + (uintptr_t)mmap->map_size;
	ret += i * mmap->desc_size;
	if(ret > limit) {
		return NULL;
	}
	return (EFI_MEMORY_DESCRIPTOR*)ret;
}

bool IsAvailable(EFI_MEMORY_DESCRIPTOR* desc) {
	EFI_MEMORY_TYPE t = desc->Type;	
	return (t == EfiBootServicesCode) ||
		(t == EfiBootServicesData) ||
		(t == EfiConventionalMemory);
}

void InitMemoryMap(UefiMemoryMap *u_mmap) {
	int desc_num = u_mmap->map_size / u_mmap->desc_size;
	int available_page = 0;
	int unavailable_page = 0;
	int page = 0;
	for(int i = 0; i < desc_num; i++) {
		EFI_MEMORY_DESCRIPTOR *desc = GetUefiMemDesc(u_mmap,i);
		page += desc->NumberOfPages;
		if(IsAvailable(desc)) {
			Print("available");
			available_page += desc->NumberOfPages;
		} else {
			Print("unavailable");
			unavailable_page += desc->NumberOfPages;
		}
	}
	Print_int("desc_num : ",desc_num, 10);
	Print_int("available : ", available_page/4/1024, 10);
	Print_int("unavailable : ", unavailable_page/4/1024, 10);
	Print_int("all : ", page/4/1024, 10);
}
