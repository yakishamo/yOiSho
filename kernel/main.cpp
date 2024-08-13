#include "../loader/types64.h"
#include "../loader/frame_info.h"

void hlt() {
	while(1) asm("hlt");
}

extern "C"
__attribute__((ms_abi))
int KernelMain(FrameInfo *frame_info){
	for(uint64_t i = 0; i < frame_info->frame_size; i++) {
		frame_info->frame_base[i] = 0xff;
	}
	hlt();
	return 0;
}
