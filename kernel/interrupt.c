#include "../common/types64.h"
#include "interrupt.h"
#include "asmfunc.h"

struct InterruptDescriptor idt[256];

void SetIDTEntry(struct InterruptDescriptor *desc, uintptr_t handler) {
  desc->offset_low = (uint16_t)handler & 0xffff;
  desc->offset_middle = (uint16_t)(handler  >> 16) & 0xffff;
  desc->offset_high = (uint16_t)(handler >> 32) & 0xffffffff;
  desc->segment_selector = GetCS();
  desc->attr.bits.interrupt_stack_table = 0;
  desc->attr.bits.gate_type = 14;
  desc->attr.bits.zero = 0;
  desc->attr.bits.dpl = 0;
  desc->attr.bits.present = 1;
}

static 
