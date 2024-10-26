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

void MarkAvailable(int i) {
	int index = (int)i/8;
	int bit = i%8;
	memory_map[index] += 1 << bit;
}

void InitMemoryMap(UefiMemoryMap *u_mmap) {
	int desc_num = u_mmap->map_size / u_mmap->desc_size;
	int pages = 0;
	memset(memory_map,0,MEMORY_MAP_SIZE);
	for(int i = 0; i < desc_num; i++) {
		EFI_MEMORY_DESCRIPTOR *desc = GetUefiMemDesc(u_mmap,i);
		if(IsAvailable(desc)) {
			int start_page = desc->PhysicalStart/(4*1024);
			pages += desc->NumberOfPages;
			for(int j = 0; j < desc->NumberOfPages;j++) {
				MarkAvailable(start_page + j);
			}
		}
	}
	Print_int("Available pages : ", pages, 10);
}
