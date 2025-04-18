#ifndef KERNEL_MALLOC_H
#define KERNEL_MALLOC_H

void InitializeKernelHeap();
void *kmalloc(uint64_t size);
void kfree(void* ptr);
void DumpHeap();

#endif /* KERNEL_MALLOC_H */
