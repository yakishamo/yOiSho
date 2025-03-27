#ifndef ASMFUNC_H
#define ASMFUNC_H

#include "../common/types64.h"

void IoOut8(uint16_t addr, uint8_t data);
uint8_t IoIn8(uint16_t addr);
void SwitchKernelStack(void* new_stack, uint64_t stack_size, void* kernel_main);
void CpuidGetVendor(char *str);
void SetDSAll(uint16_t value);
void SetCSSS(uint16_t cs, uint16_t ss);
uint64_t GetCS();
void LoadGDT(uint16_t limit, uint64_t offset);
void SetCR3(uint64_t value);
void LoadIDT(uint16_t limit, uint64_t offset);
void DebugHlt(uint64_t value1, uint64_t value2, uint64_t value3);

#endif /* ASMFUNC_H */
