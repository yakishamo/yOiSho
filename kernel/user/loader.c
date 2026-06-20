#include "../../common/types64.h"
#include "../../common/elf.h"
#include "../kprintf.h"
#include "../string.h"

void loadElf(void* buf) {
	if(memcmp(ELFMAG, buf, 3) == 0) {
		kprintf("elf format\n");
	} else {
		kprintf("!elf format\n");
	}
}
