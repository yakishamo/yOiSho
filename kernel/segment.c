#include "frame.h"
#include "descriptor.h"
#include "asmfunc.h"
#include "../common/def.h"

typedef union SegmentDescriptor SegmentDescriptor;

union SegmentDescriptor {
	uint64_t data;
	struct {
		uint64_t limit_low : 16;
		uint64_t base_low : 16;
		uint64_t base_middle : 8;
		DescriptorType type : 4;
		uint64_t system_segment : 1;
		uint64_t descriptor_privilege_level : 2;
		uint64_t present : 1;
		uint64_t limit_high : 4;
		uint64_t available : 1;
		uint64_t long_mode : 1;
		uint64_t default_operation_size : 1;
		uint64_t granularity : 1;
		uint64_t base_high : 8;
	} __attribute__((packed)) bits;
} __attribute__((packed));

void SetCodeSegment(
    SegmentDescriptor *desc,
    DescriptorType type,
    unsigned int descriptor_privilege_level,
    uint32_t base,
    uint32_t limit) {
  desc->data = 0;

  desc->bits.base_low = base & 0xffff;
  desc->bits.base_middle = (base >> 16) & 0xff;
  desc->bits.base_high = (base >> 24) & 0xff;

  desc->bits.limit_low = limit & 0xffff;
  desc->bits.limit_high = (limit >> 16) & 0xf;

  desc->bits.type = type;
  desc->bits.system_segment = 1;
  desc->bits.descriptor_privilege_level = descriptor_privilege_level;
  desc->bits.present = 1;
  desc->bits.available = 0;
  desc->bits.long_mode = 1;
  desc->bits.default_operation_size = 0;
  desc->bits.granularity = 1;
}

void SetDataSegment(
    SegmentDescriptor *desc,
    DescriptorType type,
    unsigned int descriptor_privilege_level,
    uint32_t base,
    uint32_t limit) {
  SetCodeSegment(desc,type,descriptor_privilege_level,base,limit);
  desc->bits.long_mode = 0;
  desc->bits.default_operation_size = 1;
}

static SegmentDescriptor gdt[3];

void InitializeSegment() {
  gdt[0].data = 0;
  SetCodeSegment(gdt+1, kExecuteRead, 0, 0, 0xfffff);
  SetDataSegment(gdt+2, kReadWrite, 0, 0, 0xfffff);
  LoadGDT((uint16_t)sizeof(gdt)-1, (uint64_t)gdt);

  return;
}

