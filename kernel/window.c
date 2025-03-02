#include"window.h"
#include"frame.h"

struct Window{
  unsigned int pos_x;
  unsigned int pos_y;
  unsigned int size_x;
  unsigned int size_y;
  WriteWindowPixel_t WritePixel;
  WriteWindowAscii_t WriteAscii;
  WriteWindowString_t WriteString;
  WriteWindowSquare_t WriteSquare;
  ClearWindow_t ClearWindow;
  ScrollUpWindow_t ScrollUp;
  ScrollDownWindow_t ScrollDown;
  PrintWindow_t Print;
  PrintWindow_int_t Print_int;
};

void WriteWindowPixel(Window *window, unsigned int x, unsigned int y, const Color *c) {
}

// buf size must be larger than sizeof(Window)
Window* CreateWindow(void *buf, unsigned int pos_x, unsigned int pos_y,
    unsigned int size_x, unsigned int size_y) {
  Window *window = buf;
  window->pos_x = pos_x;
  window->pos_y = pos_y;
  window->size_x = size_x;
  window->size_y = size_y;
  return window;
}

