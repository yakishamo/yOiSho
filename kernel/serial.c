#include "../common/types64.h"
#include "serial.h"
#include "asmfunc.h"
#include "kmalloc.h"

#define COM1 0x3f8
#define COM2 0x2f8
#define COM3 0x3e8
#define COM4 0x2e8

struct SERIAL_ {
	uint32_t port;
};

static uint8_t coms = 0;

SERIAL* InitializeSerial(uint32_t com) {
	int is_com_used = (coms>>(com-1))&1;
	if(is_com_used == 1) {
		return NULL;
	}
	coms = 1 << (com-1);
	int port;
	switch(com) {
		case 1:
			port = COM1;
			break;
		case 2:
			port = COM2;
			break;
		case 3:
			port = COM3;
			break;
		case 4:
			port = COM4;
			break;
		default:
			return NULL;
	}

	IoOut8(port + 1, 0x00); // IER - disable all interrupts
	IoOut8(port + 3, 0x80); // LCR - enable DLAB
	IoOut8(port + 0, 0x03); // DLL - set baud late 38400 bps (write 115200/38400 = 3)
	IoOut8(port + 1, 0x00); // DLM
	IoOut8(port + 3, 0x03); // LCR - disable DLAB, 8bit, no parity, 1 stop bit
	IoOut8(port + 2, 0xc7); // FCR - enable FIFO, clear TX/RX queues, 14bytes threshold
	IoOut8(port + 4, 0x0b); // MCR - IRQs enabled, RTS/DSR set
	IoOut8(port + 4, 0x1e); // MCR - set loopback mode, test the serial chip
	IoOut8(port + 0, 0xae); // RBR - test the serial chip (send 0xae)
	
	if(IoIn8(port + 0) != 0xae) {
		// serial fault
		return NULL;
	}

	IoOut8(port + 4, 0x0f); // MCR - set normal mode, RTS/DSR set
	SERIAL *s = kmalloc(sizeof(SERIAL));
	s->port = port;
	return s;
}

void SerialSend(SERIAL *s, uint8_t data) {
	uint32_t port = s->port;
	while((IoIn8(port+5) & 0x20) == 0); // waiting THR by watching LSR
	IoOut8(port, data);
}

uint8_t SerialReceive(SERIAL *s) {
	uint32_t port = s->port;
	if((IoIn8(port+5) & 1) == 0) {
		return 0;
	}
	return IoIn8(port);
}

void SerialPrint(SERIAL *s, char *str) {
	char *c = str;
	while(*c != '\0') {
		SerialSend(s, *c);
		c++;
	}
}
