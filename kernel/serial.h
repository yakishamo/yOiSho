#ifndef SERIAL_H
#define SERIAL_H

#include "../common/types64.h"

#define SERIAL_CLEAR "\e[2J\e[0;0H"

typedef struct SERIAL_ SERIAL;

void SerialClear(SERIAL *s);
SERIAL* InitializeSerial(uint32_t com);
void SerialSend(SERIAL *s, uint8_t data);
uint8_t SerialReceive(SERIAL *s);
uint8_t SerialReceiveNoNull(SERIAL *s);

#endif /* SERIAL_H */
