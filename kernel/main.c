#include "../common/types64.h"
#include "../common/frame_info.h"
#include "../common/def.h"
#include "frame.h"
#include "string.h"
#include "asmfunc.h"

extern FrameInfo *frame_info;

void hlt() {
	while(1) asm("hlt");
}

__attribute__((ms_abi))
int KernelMain(FrameInfo *fi){
	frame_info = fi;

	ClearScreen();
	Color c = {255,255,255, 255};
	WriteString("Hello yOiSho!!", 0, 0, &c);

	int i = 0;
	while(1) {
		if((IoIn8(0x64) & 0x01) == 1) {
			WriteString("Keyboard.", 0, i*16, &c);
			i++;
			IoIn8(0x60);
		}
	}

	hlt();
	return 0;
}
