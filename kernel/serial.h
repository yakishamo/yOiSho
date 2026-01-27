#ifndef SERIAL_H
#define SERIAL_H

#include "../common/types64.h"

#define SERIAL_CLEAR "\e[2J\e[0;0H"

typedef struct SERIAL_CONSOLE_ SERIAL_CONSOLE;

void SerialConsoleClear(SERIAL_CONSOLE *s);
SERIAL_CONSOLE* InitializeSerialConsole(uint32_t com);
uint8_t SerialReceive(SERIAL_CONSOLE *s);
uint8_t SerialReceiveNoNull(SERIAL_CONSOLE *s);
void SerialConsolePrint(SERIAL_CONSOLE *s, char *str);

#endif /* SERIAL_H */
