#include "../loader/types64.h"
#include "../loader/frame_info.h"

void hlt() {
	while(1) asm("hlt");
}

extern "C"
__attribute__((ms_abi))
int KernelMain(FrameInfo *frame_info){
	hlt();
	return 0;
}
