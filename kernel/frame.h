#ifndef FRAME_H
#define FRAME_H

#include "../common/types64.h"

typedef struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t reserved;
} Color;

extern const Color white;
extern const Color black;
extern const Color red;

void WritePixel(unsigned int x, unsigned int y, const Color *c);
void WriteAscii(const char ch, unsigned int x, unsigned int y, const Color *c);
void WriteString(const char *str, unsigned int x, unsigned int y, const Color *c);
void WriteSquare(unsigned int x1, unsigned int y1, unsigned x2, unsigned y2, const Color *c);
void ClearScreen();
void ScrollUp(unsigned int y);
void ScrollDown(unsigned int y);

#endif /* FRAME_H */
