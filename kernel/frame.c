#include "../common/frame_info.h"
#include "string.h"

#include "frame.h"

extern unsigned char kernel_hankaku[];
extern unsigned char kernel_hankaku_len;

const Color white = {255, 255, 255, 0};
const Color black = {0, 0, 0, 0};
const Color red = {255, 0, 0, 0};

FrameInfo *frame_info;

unsigned char *getFont(char c) {
	if(c*16 < kernel_hankaku_len) return NULL;
	return kernel_hankaku + c*16;
}

// RGB
void WritePixel(unsigned int x, unsigned int y, const Color *c) {
	frame_info->frame_base[(frame_info->pixel_per_scanline * y + x) * 4] = c->blue;
	frame_info->frame_base[(frame_info->pixel_per_scanline * y + x) * 4 + 1] = c->green;
	frame_info->frame_base[(frame_info->pixel_per_scanline * y + x) * 4 + 2] = c->red;
	frame_info->frame_base[(frame_info->pixel_per_scanline * y + x) * 4 + 3] = c->reserved;
}

void WriteAscii(const char ch, unsigned int x, unsigned int y, const Color *c) {
	unsigned char *font = getFont(ch);
	for(int i = 0; i < 16; i++) {
		for(int j = 0; j < 8; j++) {
			if(((font[i] >> (7-j))&0x01)) {
				WritePixel(x+j, y+i, c);
			}
		}
	}
}

void WriteString(const char *str, unsigned int x, unsigned int y, const Color *c) {
	for(int i = 0; i < strlen(str); i++) {
		WriteAscii(str[i], x+i*8, y, c);
	}
}

void WriteInteger(const char *val_name, uint64_t a, unsigned int radix, 
    unsigned int x, unsigned int y, const Color *c) {
	char str[25] = "";
	char *p = str;
	unsigned int v = a;
	int n = 1;
	while(v >= radix) {
		v/=radix;
		n++;
	}
	p = str + n;
	v = a;
	*p = 0;
	do {
		p--;
		*p = v % radix + (char)'0';
		if(*p > (char)'9') {
			*p = v % radix - 10 + 'A';
		}
		v /= radix;
	} while(p != str);
	char print_str[200] = "";
	strcat(print_str, val_name);
	strcat(print_str, str);
	WriteString(print_str, x, y, c);
}


void WriteSquare(unsigned int x1, unsigned int y1, unsigned x2, unsigned y2, const Color *c) {
	for(int j = y1; j <= y2; j++) {
		for(int i = x1; i <= x2; i++) {
			WritePixel(i,j,c);
		}
	}
}

void ClearScreen() {
	for(unsigned int i = 0; i < frame_info->frame_size; i++) {
		frame_info->frame_base[i] = 0;
	}
}

void ScrollDown(unsigned int y) {
	memmove(frame_info->frame_base + frame_info->pixel_per_scanline * 4 * y,
			frame_info->frame_base,
			frame_info->frame_size - frame_info->pixel_per_scanline * 4 * y);
}

void ScrollUp(unsigned int y) {
  memmove(frame_info->frame_base,
    frame_info->frame_base + frame_info->pixel_per_scanline * 4 * y,
    frame_info->frame_size - frame_info->pixel_per_scanline * 4 * y);
}


