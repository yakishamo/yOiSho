#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "../common/types64.h"
#include "queue.h"

#define INT_DIVIDE_BY_ZERO                0x00
#define INT_NMI_INTERRUPT                 0x02
#define INT_BREAKPOINT                    0x03
#define INT_OVERFLOW                      0x04
#define INT_BOUNDS_RANGE_EXCEEDED         0x05
#define INT_INVALID_OPECODE               0x06
#define INT_DEVICE_NOT_AVAILABLE          0x07
#define INT_DOUBLE_FAULT                  0x08
#define INT_COPROCESSOR_SEGMENT_OVERRUN   0x09
#define INT_INVALID_TSS                   0x0a
#define INT_SEGMENT_NOT_PRESENT           0x0b
#define INT_STACK_SEGMENT_FAULT           0x0c
#define INT_GENERAL_PROTECTION_FAULT      0x0d
#define INT_PAGE_FAULT                    0x0e
#define INT_X87_FPU_ERROR                 0x10
#define INT_ALIGNMENT_CHECK               0x11
#define INT_MACHINE_CHECK                 0x12
#define INT_SIMD_FLOATING_POINT_EXCEPTION 0x13
#define INT_KEYBOARD                      0x21

union InterruptDescriptorAttribute {
  uint16_t data;
  struct {
    uint16_t interrupt_stack_table : 3;
    uint16_t reserved : 5;
    uint16_t gate_type : 4;
    uint16_t zero : 1;
    uint16_t dpl : 2;
    uint16_t present : 1;
  } __attribute__((packed)) bits;
} __attribute__((packed));

struct InterruptDescriptor {
  uint16_t offset_low;
  uint16_t segment_selector;
  union InterruptDescriptorAttribute attr;
  uint16_t offset_middle;
  uint32_t offset_high;
  uint32_t reserved;
} __attribute__((packed));

struct InterruptFrame {
  uintptr_t rip;
  uintptr_t cs;
  uintptr_t flag;
  uintptr_t rsp;
  uintptr_t ss;
} __attribute__((packed));

enum InterruptType {
  IntKeyboard,
};

struct Q_InterruptMessage {
  QUEUE_ITEM queue_item;
  enum InterruptType;
  union {
    struct {
      char ascii;
    } keyboard;
  };
};

typedef struct Q_InterruptMessage Q_InterruptMessage;

extern QUEUE interrrupt_message_queue;

void SetIDTEntry(struct InterruptDescriptor *desc, uintptr_t handler);
void SetupInterrupt();

#endif /* INTERRUPT_H */
