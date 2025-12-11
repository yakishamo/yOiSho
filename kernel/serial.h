#ifndef SERIAL_H
#define SERIAL_H

#define COM1 0x3f8
#define COM2 0x2f8
#define COM3 0x3e8
#define COM4 0x2e8

#include "../common/types64.h"

void SerialInit(uint32_t port);
void SerialSend(uint32_t port, uint8_t data);
uint8_t SerialReceive(uint32_t port);
void SerialPrint(uint32_t port, char *str);

#endif /* SERIAL_H */
