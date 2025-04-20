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
#include "segment.h"
#include "window.h"
#include "asmfunc.h"
#include "paging.h"
#include "interrupt.h"
#include "kmalloc.h"

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
  InitializeSegment();
  const uint16_t kernel_cs = 1 << 3;
  const uint16_t kernel_ss = 2 << 3;
  SetDSAll(0);

  SetCSSS(kernel_cs, kernel_ss);

  SetupIdentityPaging();

  SetupInterrupt();
  
	InitializeKeycode();

	ClearScreen();

	InitializeMemoryMap(u_memory_map);

  InitializeKernelHeap();

  int *p = kmalloc(8);
  int *q = kmalloc(16);
  Print_int("p : 0x", (uint64_t)p, 16);
  Print_int("q : 0x", (uint64_t)q, 16);

  kfree(p);
  kfree(q);
  p = kmalloc(0x100);
  q = kmalloc(16);
  Print_int("p : 0x", (uint64_t)p, 16);
  Print_int("q : 0x", (uint64_t)q, 16);
  kfree(p);
  kfree(q);
  p = kmalloc(0x9);
  q = kmalloc(0x80);
  int *r = kmalloc(0x1);
  Print_int("p : 0x", (uint64_t)p, 16);
  Print_int("q : 0x", (uint64_t)q, 16);
  Print_int("r : 0x", (uint64_t)r, 16);

	terminal_v2();

	hlt();
	return 0;
}
