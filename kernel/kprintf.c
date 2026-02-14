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

void vkprintf(char *fmt, va_list ap) {
	int len = strlen(fmt) + 1024;
	char *buf = kmalloc(len);
	vsnprintf(buf, len, fmt, ap);
	kprint(buf);
	kfree(buf);
}

// limit: output size < fmt + 1024
void kprintf(char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vkprintf(fmt, ap);
	va_end(ap);
}
