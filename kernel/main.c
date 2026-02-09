#include "../common/types64.h"
#include "../common/frame_info.h"
#include "../common/def.h"
#include "../common/memory_map.h"
#include "frame.h"
#include "string.h"
#include "asmfunc.h"
#include "keyboard.h"
#include "file.h"
#include "parse.h"
#include "memory.h"
#include "segment.h"
#include "asmfunc.h"
#include "paging.h"
#include "interrupt/interrupt.h"
#include "kmalloc.h"
#include "pic.h"
#include "serial.h"
#include "kprintf.h"
#include "fat.h"

#define KERNEL_STACK_SIZE 1024*1024

extern FrameInfo *frame_info;
UefiMemoryMap *u_memory_map;
void *VOLUME_IMAGE = NULL;
int KernelMain();
uint16_t kernel_stack[KERNEL_STACK_SIZE/2];

void hlt() {
	while(1) __asm__("hlt");
}

__attribute__((ms_abi))
void KernelEntryPoint(FrameInfo *fi, UefiMemoryMap *memmap, void* volume_image) {
	frame_info = fi;
	u_memory_map = memmap;
	VOLUME_IMAGE = volume_image;
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
  // InitializePic();
  
	InitializeKeycode();

	ClearScreen();

	InitializeMemoryMap(u_memory_map);

  InitializeKernelHeap();

	SERIAL_CONSOLE *serial_com1 = InitializeSerialConsole(1);

	setKprintfSerial(serial_com1);

	FatFileSystem fat = loadFat(VOLUME_IMAGE);

	kprintf("Hello, yOiSho!!\r\n");
	printVolume(fat);
	char *str = "sequence s test";
	kprintf("hogehoge %s %s\r\n", str, str);
	kprintf("str: 0x%x dayo!\r\n", str);
	kprintf("1 + 2 = %d\n\r", 1 + 2);
	kprintf("frame_info: 0x%X\r\n0x%x 0x%x 0x%X %s\r\n",
			frame_info, 0xdeadbeef, 0xcafebabe, 0xcafebabe, "\r\nwhat is cafebabe??");
	kprintf("kenrel end!!!\r\n");

	hlt();
	return 0;
}
