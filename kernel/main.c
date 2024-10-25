#include "../common/types64.h"
#include "../common/frame_info.h"
#include "../common/def.h"
#include "../common/memory_map.h"
#include "frame.h"
#include "string.h"
#include "asmfunc.h"
#include "keyboard.h"
#include "file.h"
#include "cursor.h"
#include "parse.h"
#include "editor.h"
#include "terminal.h"
#include "memory.h"

#define KERNEL_STACK_SIZE 1024*1024

extern FrameInfo *frame_info;
UefiMemoryMap *u_memory_map;
int KernelMain();
uint16_t kernel_stack[KERNEL_STACK_SIZE/2];

void hlt() {
	while(1) asm("hlt");
}

__attribute__((ms_abi))
void KernelEntryPoint(FrameInfo *fi, UefiMemoryMap *memmap) {
	frame_info = fi;
	u_memory_map = memmap;
	SwitchKernelStack(kernel_stack, KERNEL_STACK_SIZE, KernelMain);
}

int KernelMain(){

	InitializeKeycode();

	ClearScreen();

	InitMemoryMap(u_memory_map);

	terminal();

	hlt();
	return 0;
}
