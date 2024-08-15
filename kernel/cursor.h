#ifndef CURSOR_H
#define CURSOR_H

#include "frame.h"

typedef struct _CURSOR CURSOR;

struct _CURSOR {
	unsigned int x;
	unsigned int y;
	Color c;
};

CURSOR *InitializeCursor(CURSOR *cursor, const Color *c);
void PrintCursor(CURSOR *cur);
void EraseCursor(CURSOR *cur);
void MoveCursor(CURSOR *cur, unsigned int x, unsigned int y);
void CursorNext(CURSOR *cur);
void CursorBack(CURSOR *cur);

#endif /* CURSOR_H */
