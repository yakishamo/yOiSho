#include "../../common/types64.h"
#include "../asmfunc.h"
#include "../frame.h"
#include "../string.h"

#include "interrupt.h"
#include "message.h"

#define DEFINE_DEFAULT_INT(interrupt_name) \
  __attribute__((interrupt)) \
  static void (interrupt_name)(InterruptFrame *frame) { \
    while(1) __asm__("hlt"); \
    NotifyEndOfInterrupt(); \
  }

/*
#define DEFINE_DEFAULT_INT(interrupt_name) \
  __attribute__((interrupt)) \
  static void (interrupt_name)(InterruptFrame *frame) { \
    char *str = #interrupt_name ; \
    WriteSquare(0,0,8*100,20,&red); \
    WriteString(str, 0,0, &white); \
    DebugHlt(frame->rip, frame->rsp, frame->flag); \
    NotifyEndOfInterrupt(); \
  }
  */

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

__attribute__((no_caller_saved_registers))
static void hlt() {
  while(1) __asm__("hlt");
}

__attribute__((no_caller_saved_registers))
static void NotifyEndOfInterrupt() {
  volatile uint32_t *end_of_interrupt = (uint32_t*)0xfee000b0; 
  *end_of_interrupt = 0;
}

DEFINE_DEFAULT_INT(DivideByZero);
DEFINE_DEFAULT_INT(NmiInterrupt);
DEFINE_DEFAULT_INT(BreakPoint);
DEFINE_DEFAULT_INT(Overflow);
DEFINE_DEFAULT_INT(BoundsRangeExceeded);
DEFINE_DEFAULT_INT(InvalidOpecode);
DEFINE_DEFAULT_INT(DeviceNotAvailable);
DEFINE_DEFAULT_INT(DoubleFault);
DEFINE_DEFAULT_INT(CoprocessorSegmentOverrun);
DEFINE_DEFAULT_INT(InvalidTss);
DEFINE_DEFAULT_INT(SegmentNotPresent);
DEFINE_DEFAULT_INT(StackSegmentFault);
DEFINE_DEFAULT_INT(GeneralProtection);
DEFINE_DEFAULT_INT(PageFault);

// DEFINE_DEFAULT_INT(KeyboardInterrupt);
__attribute__((interrupt))
static void KeyboardInterrupt(InterruptFrame *frame) {
  NotifyEndOfInterrupt();
}

IMQueue message_queue;
static IM im_queue_buf[IMQUEUE_MAX];
void SetupInterrupt() {
  IMQueueInit(&message_queue, im_queue_buf, IMQUEUE_MAX);
  SetIDTEntry(&idt[INT_DIVIDE_BY_ZERO], (uintptr_t)DivideByZero);
  SetIDTEntry(&idt[INT_NMI_INTERRUPT], (uintptr_t)NmiInterrupt);
  SetIDTEntry(&idt[INT_BREAKPOINT], (uintptr_t)BreakPoint);
  SetIDTEntry(&idt[INT_OVERFLOW], (uintptr_t)Overflow);
  SetIDTEntry(&idt[INT_BOUNDS_RANGE_EXCEEDED], (uintptr_t)BoundsRangeExceeded);
  SetIDTEntry(&idt[INT_INVALID_OPECODE], (uintptr_t)InvalidOpecode);
  SetIDTEntry(&idt[INT_DEVICE_NOT_AVAILABLE], (uintptr_t)DeviceNotAvailable);
  SetIDTEntry(&idt[INT_DOUBLE_FAULT], (uintptr_t)DoubleFault);
  SetIDTEntry(&idt[INT_COPROCESSOR_SEGMENT_OVERRUN], (uintptr_t)CoprocessorSegmentOverrun);
  SetIDTEntry(&idt[INT_SEGMENT_NOT_PRESENT], (uintptr_t)SegmentNotPresent);
  SetIDTEntry(&idt[INT_STACK_SEGMENT_FAULT], (uintptr_t)StackSegmentFault);
  SetIDTEntry(&idt[INT_GENERAL_PROTECTION_FAULT], (uintptr_t)GeneralProtection);
  SetIDTEntry(&idt[INT_PAGE_FAULT], (uintptr_t)PageFault);
  SetIDTEntry(&idt[INT_KEYBOARD], (uintptr_t)KeyboardInterrupt);
  LoadIDT(255, (uint64_t)idt);
  __asm__("sti");
  return;
}
