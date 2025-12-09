#include "../common/memory_map.h"
#include "../uefi/uefi_memory_map.h"
#include "../uefi/uefi_memory.h"
#include "memory.h"
#include "string.h"
#include "frame.h"
#include "terminal.h"

#define MEMORY_MAP_SIZE 1024*1024/8 * 4

// 1 : used
// 0 : free
uint8_t memory_map[MEMORY_MAP_SIZE];
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

static inline void SetBit(int page, int bit) {
	int index = (int)page/8;
	int bit_index = page%8;
  if(index >= MEMORY_MAP_SIZE) {
    WriteString("Over", 0, 16, &red);
    WriteInteger("Page : 0x", page, 16, 0, 32, &red);
    while(1) __asm__("hlt");
  }
  if(bit == 0) {
    uint8_t mask = ~(0x01 << bit_index);
    memory_map[index] &= mask;
  } else {
    memory_map[index] |= (0x01 << bit_index); 
  }
}

static inline bool GetBit(unsigned int page) {
	int index = (int)page/8;
	int bit = page%8;
	return (memory_map[index]>>bit)&1;
}

void InitializeMemoryMap(UefiMemoryMap *u_mmap) {
	uintptr_t desc_num = u_mmap->map_size / u_mmap->desc_size;
	uintptr_t pages = 0;
	memset(memory_map,0xff,MEMORY_MAP_SIZE);

	for(uintptr_t i = 0; i < desc_num; i++) {
		EFI_MEMORY_DESCRIPTOR *desc = GetUefiMemDesc(u_mmap,i);
		if(IsAvailable(desc)) {
	    page_max = MAX(page_max,(uintptr_t)desc->PhysicalStart/0x1000 + desc->NumberOfPages);
			uintptr_t start_page = desc->PhysicalStart/0x1000;
			pages += desc->NumberOfPages;
			for(uintptr_t j = 0; j < desc->NumberOfPages;j++) {
				SetBit(start_page + j, 0);
      }
		}
	}
	Print_int("Available memory : 0x", pages * 0x1000, 16);
}

// when page is not allocated, return 0
uint64_t AllocatePage(uint64_t page_size) {
	uint64_t page = 1;
	uint64_t i = 0;
	while((i < page_size) && (page < MEMORY_MAP_SIZE)) {
		if(!GetBit(page)) {
			i++;
		} else {
			i = 0;
		}
		page++;
	}
	if(i == page_size) {
		uintptr_t ret_page = page - page_size - 1;
		for(int j = 0; j < page_size; j++) {
			SetBit(ret_page+i,1);
		}
		return ret_page * 0x1000;
	}
	return 0;
}

uint64_t GetAvailablePages() {
  uint64_t available_pages = 0;
  Print_int("MEMORY_MAP_SIZE*8 : 0x", MEMORY_MAP_SIZE*8, 16);
  for(uint64_t i = 0; i < MEMORY_MAP_SIZE*8; i++) {
    if(GetBit(i) == 0) {
      available_pages++;
    }
  }
  return available_pages;
}

uint64_t GetPageMax() {
  return page_max;
}
