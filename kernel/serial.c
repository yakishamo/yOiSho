#include "../common/types64.h"
#include "serial.h"
#include "asmfunc.h"

void SerialInit(uint32_t port) {
	IoOut8(port + 1, 0x00);
	IoOut8(port + 3, 0x80);
	IoOut8(port + 0, 0x03);
	IoOut8(port + 1, 0x00);
	IoOut8(port + 3, 0x03);
	IoOut8(port + 2, 0xc7);
	IoOut8(port + 4, 0x0b);
	IoOut8(port + 4, 0x1e);
	IoOut8(port + 0, 0xae);

	if(IoIn8(port + 0) != 0xae) {
		return;
	}

	IoOut8(port + 4, 0x0f);
	return;
}

void SerialSend(uint32_t port, uint8_t data) {
	while((IoIn8(port+5) & 0x20) == 0);
	IoOut8(port, data);
}

uint8_t SerialReceive(uint32_t port) {
	if((IoIn8(port+5) & 1) == 0) {
		return 0;
	}
	return IoIn8(port);
}

void SerialPrint(uint32_t port, char *str) {
	char *c = str;
	while(*c != '\0') {
		SerialSend(port, *c);
		c++;
	}
}
