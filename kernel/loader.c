#include "../common/types64.h"
#include "../common/elf.h"
#include "kprintf.h"
#include "string.h"

void loadElf(void* buf) {
	if(memcmp(buf, ELFMAG, 4) == 0) {
		kprintf("buf is elf format\n");
	} else {
		kprintf("buf is NOT elf format.\n");
	}
}
