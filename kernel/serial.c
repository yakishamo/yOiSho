#include "../common/types64.h"
#include "serial.h"
#include "asmfunc.h"
#include "kmalloc.h"
#include "string.h"

#define COM1 0x3f8
#define COM2 0x2f8
#define COM3 0x3e8
#define COM4 0x2e8

// Cursor on serial
//
typedef struct SERIAL_CURSOR {
	uint64_t x;
	uint64_t y;
} SERIAL_CURSOR;

struct SERIAL_CONSOLE_ {
	uint32_t port;
	SERIAL_CURSOR cursor;
};

static uint8_t used_coms = 0;
SERIAL_CONSOLE* InitializeSerialConsole(uint32_t com) {
	int is_com_used = (used_coms>>(com-1))&1;
	if(is_com_used == 1) {
		return NULL;
	}
	used_coms = 1 << (com-1);
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
	IoOut8(port + 0, 0x01); // DLL - set baud late 115200 bps
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
	SERIAL_CONSOLE *s = kmalloc(sizeof(SERIAL_CONSOLE));
	s->port = port;
	return s;
}

static void SerialConsoleSendRaw(SERIAL_CONSOLE *s, uint8_t data) {
	uint32_t port = s->port;
	while((IoIn8(port+5) & 0x20) == 0); // waiting THR by watching LSR
	IoOut8(port, data);
}

uint8_t SerialConsoleReceive(SERIAL_CONSOLE *s) {
	uint32_t port = s->port;
	if((IoIn8(port+5) & 1) == 0) {
		return 0;
	}
	return IoIn8(port);
}

uint8_t SerialConsoleReceiveNoNull(SERIAL_CONSOLE *s) {
	char c = '\0';
	while((c = SerialConsoleReceive(s)) == '\0');
	return c;
}

// this doesn't move SERIAL_CURSOR in SERIAL_CONSOLE 
static void SerialConsolePrintRaw(SERIAL_CONSOLE *s, char *str) {
	char *c = str;
	while(*c != '\0') {
		SerialConsoleSendRaw(s, *c);
		c++;
	}
}

void SerialConsoleClear(SERIAL_CONSOLE *s) {
	s->cursor.x = 0;
	s->cursor.y = 0;
	SerialConsolePrintRaw(s, SERIAL_CLEAR);
}

// Processes escape sequence and compute cursor location
// It returns sequence size
static uintptr_t process_esc_seq(SERIAL_CURSOR *cur, char *start) {
	if(*start != '\e') {
		// *start should be '\e'
		return 0;
	}
	char *c = start;
	c++;
	if(*c == '[') {
		c++;
		int seq_param = 0;
		while(*c>='0' && *c<='9') {
			seq_param *= 10;
			seq_param += ASCII_TO_NUM(*c);
			c++;
		}
		switch(*c) {
			case 'A':
				cur->y -= 1;
				break;
			case 'B':
				cur->y += 1;
				break;
			case 'C':
				cur->x += 1;
				break;
			case 'D':
				cur->x -= 1; 
				break;
			default:
				// unhandled sequence
				break;
		}
		return (uintptr_t)c - (uintptr_t)start + 1;
	} else {
		// Parameter start is not escape sequence
		// Escape sequence starts "\e["
		return 1 ;
	}
}


void SerialConsolePrint(SERIAL_CONSOLE *s, char *str) {
	for(char *c = str; *c != '\0'; c++) {
		if(*c == '\e') {
			uintptr_t seq_size = process_esc_seq(&(s->cursor), c);
			for(uintptr_t i = 0; i < seq_size; i++) {
				SerialConsoleSendRaw(s, *c);
				c++;
			}
		} else {
			SerialConsoleSendRaw(s, *c);
			s->cursor.x += 1;
		}
	}
}
