#include "../loader/types64.h"
#include "../loader/frame_info.h"

FrameInfo *frame_info;

typedef struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t reserved;
} Color;

void WritePixelRGB(uint32_t x, uint32_t y, Color c) {
	frame_info->frame_base[frame_info->pixel_per_scanline * 4 * y + x*4] = c.red;
	frame_info->frame_base[frame_info->pixel_per_scanline * 4 * y + x*4+1] = c.green;
	frame_info->frame_base[frame_info->pixel_per_scanline * 4 * y + x*4+2] = c.blue;
	frame_info->frame_base[frame_info->pixel_per_scanline * 4 * y + x*4+3] = c.reserved;
}

void WritePixelBGR(uint32_t x, uint32_t y, Color c) {
	frame_info->frame_base[frame_info->pixel_per_scanline * 4 * y + x*4] = c.blue;
	frame_info->frame_base[frame_info->pixel_per_scanline * 4 * y + x*4+1] = c.green;
	frame_info->frame_base[frame_info->pixel_per_scanline * 4 * y + x*4+2] = c.red;
	frame_info->frame_base[frame_info->pixel_per_scanline * 4 * y + x*4+3] = c.reserved;
}

void hlt() {
	while(1) asm("hlt");
}

extern "C"
__attribute__((ms_abi))
int KernelMain(FrameInfo *fi){
	frame_info = fi;

	if(frame_info->format == RGB) {
		for(int i = 0; i < 100; i++) {
			WritePixelRGB(i,i,{100,100,100});
		}
	} else {
		for(int i = 0; i < 100; i++) {
			WritePixelBGR(i,i,{200,200,200});
		}
	}
	hlt();
	return 0;
}
