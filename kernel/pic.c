#include "asmfunc.h"

#define PIC0_ICW1		0x0020
#define PIC0_OCW2		0x0020
#define PIC0_IMR		0x0021
#define PIC0_ICW2		0x0021
#define PIC0_ICW3		0x0021
#define PIC0_ICW4		0x0021
#define PIC1_ICW1		0x00a0
#define PIC1_OCW2		0x00a0
#define PIC1_IMR		0x00a1
#define PIC1_ICW2		0x00a1
#define PIC1_ICW3		0x00a1
#define PIC1_ICW4		0x00a1

void InitializePic() {
  IoOut8(PIC0_IMR, 0xff);
  IoOut8(PIC1_IMR, 0xff);

  IoOut8(PIC0_ICW1, 0x11); // edge trigger mode
  IoOut8(PIC0_ICW2, 0x20); // IRQ 0-7, INT 0x20-0x27
  IoOut8(PIC0_ICW3, 1 << 2);
  IoOut8(PIC0_ICW4, 0x01); //non buffer mode

  IoOut8(PIC1_ICW1, 0x11);
  IoOut8(PIC1_ICW2, 0x28);
  IoOut8(PIC1_ICW3, 2);
  IoOut8(PIC1_ICW4, 0x01);

  IoOut8(PIC0_IMR, 0xfd);
  IoOut8(PIC1_IMR, 0xff);
}
