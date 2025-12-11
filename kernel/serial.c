#include "../common/types64.h"
#include "serial.h"
#include "asmfunc.h"

void SerialInit(uint32_t port) {
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
		return;
	}

	IoOut8(port + 4, 0x0f); // MCR - set normal mode, RTS/DSR set
	return;
}

void SerialSend(uint32_t port, uint8_t data) {
	while((IoIn8(port+5) & 0x20) == 0); // waiting THR by watching LSR
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
