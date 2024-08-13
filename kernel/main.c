#include "../common/types64.h"
#include "../common/frame_info.h"
#include "../common/def.h"

FrameInfo *frame_info;

typedef struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t reserved;
} Color;

// RGB
void WritePixel(uint32_t x, uint32_t y, Color c) {
	frame_info->frame_base[(frame_info->pixel_per_scanline * y + x) * 4] = c.blue;
	frame_info->frame_base[(frame_info->pixel_per_scanline * y + x) * 4 + 1] = c.green;
	frame_info->frame_base[(frame_info->pixel_per_scanline * y + x) * 4 + 2] = c.red;
	frame_info->frame_base[(frame_info->pixel_per_scanline * y + x) * 4 + 3] = c.reserved;
}

void hlt() {
	while(1) asm("hlt");
}

__attribute__((ms_abi))
int KernelMain(FrameInfo *fi){
	frame_info = fi;

	Color c = {255,0,0,0};
	for(uint32_t i = 0; i < 500; i++) {
		WritePixel(i,i,c);
	}
	hlt();
	return 0;
}
