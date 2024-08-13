#ifndef FRAME_H
#define FRAME_H

#include "../common/types64.h"

typedef struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t reserved;
} Color;

void WritePixel(unsigned int x, unsigned int y, const Color *c);
void WriteAscii(char ch, unsigned int x, unsigned int y, const Color *c);
void WriteString(char *str, unsigned int x, unsigned int y, Color *c);
void WriteSquare(unsigned int x1, unsigned int y1, unsigned x2, unsigned y2, Color *c);
void ClearScreen();

#endif /* FRAME_H */
