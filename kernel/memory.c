#include "../common/memory_map.h"
#include "../uefi/uefi_memory_map.h"
#include "../uefi/uefi_memory.h"
#include "memory.h"
#include "string.h"
#include "frame.h"

// 1 : available
// 0 : not available
#define MEMORY_MAP_SIZE 1024*1024/8

char memory_map[MEMORY_MAP_SIZE];
int page_max = 0;

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

void SetBit(int page, int bit) {
	int index = (int)page/8;
	int bit_index = page%8;
	memory_map[index] |= (bit == 0 ? 0 : 1) << bit_index;
}

static bool GetBit(unsigned int page) {
	int index = (int)page/8;
	int bit = page%8;
	return (memory_map[index]>>bit)&1;
}

void InitMemoryMap(UefiMemoryMap *u_mmap) {
	uintptr_t desc_num = u_mmap->map_size / u_mmap->desc_size;
	uintptr_t pages = 0;
	memset(memory_map,0,MEMORY_MAP_SIZE);
	for(uintptr_t i = 0; i < desc_num; i++) {
		EFI_MEMORY_DESCRIPTOR *desc = GetUefiMemDesc(u_mmap,i);
		page_max = MAX(page_max,(uintptr_t)desc->PhysicalStart/(4*1024) + desc->NumberOfPages);
		if(IsAvailable(desc)) {
			uintptr_t start_page = desc->PhysicalStart/(4*1024);
			pages += desc->NumberOfPages;
			for(uintptr_t j = 0; j < desc->NumberOfPages;j++) {
				SetBit(start_page + j, 1);
			}
		}
	}
	Print_int("Available pages : 0x", pages, 16);
}

// when page is not allocated, return 0
uint64_t AllocatePage(uint64_t page_size) {
	uint64_t page = 1;
	uint64_t i = 0;
	while((i < page_size) && (page < MEMORY_MAP_SIZE)) {
		if(GetBit(page)) {
			i++;
		} else {
			i = 0;
		}
		page++;
	}
	if(i == page_size) {
		uintptr_t ret_page = page - page_size;
		for(int j = 0; j < page_size; j++) {
			SetBit(ret_page+i,0);
		}
		return ret_page;
	}
	return 0;
}
