#ifndef SERIAL_H
#define SERIAL_H

#include "../common/types64.h"

typedef struct SERIAL_ SERIAL;

SERIAL* InitializeSerial(uint32_t com);
void SerialSend(SERIAL *s, uint8_t data);
uint8_t SerialReceive(SERIAL *s);
void SerialPrint(SERIAL *s, char *str);

#endif /* SERIAL_H */
