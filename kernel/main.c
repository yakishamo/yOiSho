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

extern FrameInfo *frame_info;

void hlt() {
	while(1) asm("hlt");
}

__attribute__((ms_abi))
int KernelMain(FrameInfo *fi, MemoryMap *memory_map){

	frame_info = fi;
	InitializeKeycode();

	ClearScreen();

	terminal();

	hlt();
	return 0;
}
