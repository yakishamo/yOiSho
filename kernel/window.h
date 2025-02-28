#ifndef WINDOW_H
#define WINDOW_H

#include "frame.h"

typedef struct Window Window;

typedef void (*WriteWindowPixel_t)(Window *window,
    unsigned int x, unsigned int y, const Color *c);
typedef void (*WriteWindowAscii_t)(Window *window, const char ch,
    unsigned int x, unsigned int y, const Color *c);
typedef void (*WriteWindowString_t)(Window *window, const char *str,
    unsigned int x, unsigned int y, const Color *c);
typedef void (*WriteWindowSquare_t)(Window *window,
    unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, const Color *c);
typedef void (*ClearWindow_t)(Window *window);
typedef void (*ScrollUpWindow_t)(Window *window, unsigned int y);
typedef void (*ScrollDownWindow_t)(Window *window, unsigned int y);
typedef void (*PrintWindow_t)(Window *window, const char* str);
typedef void (*PrintWindow_int_t)(Window *window, const char *val_name, uint64_t a, unsigned int radix);

#endif /* WINDOW_H */
