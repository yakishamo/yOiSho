#ifndef ASMFUNC_H
#define ASMFUNC_H

void IoOut8(uint16_t addr, uint8_t data);
uint8_t IoIn8(uint16_t addr);
void SwitchKernelStack(void* new_stack, uint64_t stack_size, void* kernel_main);

#endif /* ASMFUNC_H */
