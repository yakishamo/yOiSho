#ifndef KPRINTF_H
#define KPRINTF_H

#include "serial.h"

void setKprintfSerial(SERIAL_CONSOLE *s);
void removeKprintfSerial();
extern void kprint(char *str);
extern void kprintf(char *fmt, ...);

#endif /* KPRINTF_H */
