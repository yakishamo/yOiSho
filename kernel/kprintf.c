#include "string.h"
#include "serial.h"
#include "stdarg.h"
#include "kmalloc.h"

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

void kprintc(char c) {
	if(KPRINTF_SERIAL == NULL) {
		return;
	}
	sendSerialConsole(KPRINTF_SERIAL, c);
}

// support %s, %c, %d, %x
// When kprintf find other sequence(%*),
// it ignore * and do not output *
void kprintf(char *fmt, ...) {
	va_list ap;

	va_start(ap, fmt);

	for(char *c = fmt; *c != '\0'; c++) {
		if(*c == '%') {
			c++;
			if(*c == 's') {
				char* str = va_arg(ap, char*);
				kprint(str);
			} else if(*c == 'c') {
				char c = va_arg(ap, int);
				kprintc(c);
			} else if(*c == 'd') {
				int i = va_arg(ap, int);
				char *str = kmalloc(64);
				itoa(str, i, 10);
				kprint(str);
				kfree(str);
			} else if(*c == 'x') {
				int i = va_arg(ap, int);
				char *str = kmalloc(64);
				itoa(str, i, 16);
				kprint(str);
				kfree(str);
			} else if(*c == '\0') {
				break;
			}
		} else {
			kprintc(*c);
		}
	}
}
