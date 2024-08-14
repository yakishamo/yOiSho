#ifndef CURSOR_H
#define CURSOR_H

typedef struct _CURSOR CURSOR;

CURSOR *InitializeCursor(Color *c);
void PrintCursor(CURSOR *cur);
void EraseCursor(CURSOR *cur);
void MoveCursor(CURSOR *cur, unsigned int x, unsigned int y);
void CursorNext(CURSOR *cur);
void CursorBack(CURSOR *cur);

#endif /* CURSOR_H */
