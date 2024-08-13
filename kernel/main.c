#include "../common/types64.h"
#include "../common/frame_info.h"
#include "../common/def.h"

extern unsigned char kernel_hankaku[];
extern unsigned char kernel_hankaku_len;

FrameInfo *frame_info;

unsigned char *getFont(char c) {
	if(c*16 < kernel_hankaku_len) return NULL;
	return kernel_hankaku + c*16;
}

typedef struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t reserved;
} Color;

// RGB
void WritePixel(unsigned int x, unsigned int y, const Color *c) {
	frame_info->frame_base[(frame_info->pixel_per_scanline * y + x) * 4] = c->blue;
	frame_info->frame_base[(frame_info->pixel_per_scanline * y + x) * 4 + 1] = c->green;
	frame_info->frame_base[(frame_info->pixel_per_scanline * y + x) * 4 + 2] = c->red;
	frame_info->frame_base[(frame_info->pixel_per_scanline * y + x) * 4 + 3] = c->reserved;
}

void WriteAscii(char ch, unsigned int x, unsigned int y, const Color *c) {
	unsigned char *font = getFont(ch);
	for(int i = 0; i < 16; i++) {
		for(int j = 0; j < 8; j++) {
			if(((font[i] >> (8-j))&0x01) == 1) {
				WritePixel(x+j, y+i, c);
			}
		}
	}
}

int strlen(char *str) {
	int i = 0;
	while(str[i] != '\0') i++;
	return i;
}

void WriteString(char *str, unsigned int x, unsigned int y, Color *c) {
	for(int i = 0; i < strlen(str); i++) {
		WriteAscii(str[i], x+i*8, y, c);
	}
}

void hlt() {
	while(1) asm("hlt");
}

__attribute__((ms_abi))
int KernelMain(FrameInfo *fi){
	frame_info = fi;

	Color c = {255,0,0,0};
	for(uint32_t i = 0; i < 500; i++) {
		WritePixel(i,i,&c);
	}

	c.green = 255;
	c.blue = 255;
	WriteAscii('A', 0, 0, &c);

	WriteString("Hello yOiSho!!", 0, 16, &c);

	hlt();
	return 0;
}
