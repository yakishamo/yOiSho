#include "../common/types64.h"
#include "../elf.h"

void loadElf(void* buf) {
	if(memcmp(buf, ELFMAG, 4) == 0) {
		kprintf("buf is rlf format\n");
	} else {
		kprintf("buf is NOT elf format.\n");
	}
}
