#include "frame.h"
#include "descriptor.h"
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

void InitializeSegment() {
  SegmentDescriptor gdt[4];
  gdt[0].data = 0;

  // 

  return;
}
