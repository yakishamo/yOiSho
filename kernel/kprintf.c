#include "string.h"
#include "serial.h"

#include "kprintf.h"

static SERIAL_CONSOLE *KPRINTF_SERIAL = NULL;

void setKprintfSerial(SERIAL_CONSOLE *s) {
	KPRINTF_SERIAL = s;
	SerialConsoleClear(KPRINTF_SERIAL);
}

void removeKprintfSerial() {
	KPRINTF_SERIAL = NULL;
} 

void kprint(char *str) {
	if(KPRINTF_SERIAL == NULL) {
		return;
	}
	SerialConsolePrint(KPRINTF_SERIAL, str);
}
