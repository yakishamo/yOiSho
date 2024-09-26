#include "../common/types64.h"

#include "frame.h"
#include "cursor.h"

#define CURSOR_XSIZE 8
#define CURSOR_YSIZE 2

CURSOR *InitializeCursor(CURSOR *cursor, const Color *c) {
	cursor->x = 0;
	cursor->y = 0;
	cursor->c.red = c->red;
	cursor->c.green = c->green;
	cursor->c.blue = c->blue;
	cursor->c.reserved = c->reserved;
	return cursor;
}

void PrintCursor(CURSOR *cur) {
	WriteSquare(cur->x, cur->y+16, 
			cur->x + CURSOR_XSIZE, 
			cur->y + CURSOR_YSIZE+16,
			&cur->c);
}

void EraseCursor(CURSOR *cur) {
	Color black = {0,0,0,0};
	WriteSquare(cur->x, cur->y+16,
			cur->x + CURSOR_XSIZE,
			cur->y + CURSOR_YSIZE+16,
			&black);
}	

void MoveCursor(CURSOR *cur, unsigned int x, unsigned int y) {
	cur->x = x;
	cur->y = y;
}

void CursorNext(CURSOR *cur) {
	EraseCursor(cur);
	MoveCursor(cur, cur->x + CURSOR_XSIZE, cur->y);
	PrintCursor(cur);
}

void CursorBack(CURSOR *cur) {
	EraseCursor(cur);
	MoveCursor(cur, cur->x - CURSOR_XSIZE, cur->y);
	PrintCursor(cur);
}
