#ifndef TERMINAL_H
#define TERMINAL_H

#include "../common/types64.h"

void terminal();
void terminal_v2();
void terminal_serial(uint32_t port);
void Print(const char* str);
void Print_int(const char *val_name, uint64_t a, unsigned int radix);

#endif /* TERMINAL_H */
