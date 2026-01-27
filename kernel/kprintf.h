#ifndef KPRINTF_H
#define KPRINTF_H

#include "serial.h"

void setKprintfSerial(SERIAL_CONSOLE *s);
void removeKprintfSerial();
void kprint(char *str);

#endif /* KPRINTF_H */
