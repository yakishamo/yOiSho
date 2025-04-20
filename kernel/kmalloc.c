#include "../common/types64.h"
#include "terminal.h"
#include "memory.h"
#include "string.h"

#define KERNEL_HEAP_FRAMES 64*512
#define KMALLOC_CHUNK_MAGIC "ChunkHead"

typedef struct ChunkHead ChunkHead;
typedef union ChunkFlag ChunkFlag;

static uint8_t *kernel_heap = NULL;
static ChunkHead *chunk_root = NULL;

union ChunkFlag {
  uint64_t data;
  struct {
    uint64_t available : 1;
    uint64_t reserved : 63;
  } bits __attribute__((packed));
} __attribute__((packed));

struct ChunkHead {
  char magic[16];
  ChunkHead *next;
  uint64_t size;
  ChunkFlag flags;
  uint8_t chunk[];
} __attribute__((packed));

static void InitChunk(ChunkHead *ch, ChunkHead *next, uint64_t size, uint64_t available) {
  strcpy(ch->magic, KMALLOC_CHUNK_MAGIC);
  ch->next = next;
  ch->size = size;
  ch->flags.bits.available = available & 1;
  memset(&ch->chunk, 0, size);
}

void InitializeKernelHeap(){
  kernel_heap = (uint8_t*)AllocatePage(KERNEL_HEAP_FRAMES);
  if(kernel_heap == NULL) {
    asm("int3");
  }
  Print("kernel heap allocated.");
  Print_int("kernel_heap : 0x", (uintptr_t)kernel_heap, 16);
  chunk_root = (ChunkHead*)kernel_heap;
  InitChunk(chunk_root, NULL, KERNEL_HEAP_FRAMES * 0x100 - sizeof(ChunkHead), 1);
  Print_int("sizeof(ChunkHead) : 0x", sizeof(ChunkHead), 16);
}

void *kmalloc(uint64_t size) {
  ChunkHead *iter = chunk_root;
  if(size%8 != 0) {
    size = size - size%8 + 8;
  }
  for(;;) {
    if(iter == NULL) {
      return NULL;
    }
    if(iter->flags.bits.available == 1) {
      if(iter->size >= size + sizeof(ChunkHead)) {
        ChunkHead *new = (ChunkHead*)(((uintptr_t)iter) + sizeof(ChunkHead) + size);
        InitChunk(new, iter->next, iter->size - sizeof(ChunkHead) - size, 1);
        InitChunk(iter, new, size, 0);
        return (void*)iter->chunk;
      } else {
        ChunkHead *iter2 = iter->next;
        uint64_t available_size = iter->size;
        for(;;) {
          if(iter2 == NULL) {
            return NULL;
          }
          available_size += iter2->size;
          if(iter2->flags.bits.available == 0) {
            iter = iter2->next;
            break;
          } else if(available_size < size + sizeof(ChunkHead)) {
            available_size += sizeof(ChunkHead);
            iter2 = iter2->next;
          } else {
            ChunkHead *new = (ChunkHead*)(((uintptr_t)iter) + sizeof(ChunkHead) + size);
            InitChunk(new, iter2->next, available_size - sizeof(ChunkHead) - size, 1);
            InitChunk(iter, new, size, 0);
            return (void*)iter->chunk;
          }
        }
      }
    } else {
      iter = iter->next;
    }
  }
}

void kfree(void *ptr) {
  ChunkHead *ch = (ChunkHead*)(((uintptr_t)ptr) - sizeof(ChunkHead));
  if(strcmp(ch->magic, KMALLOC_CHUNK_MAGIC) != 0) {
    Print("kfree failed.");
    asm("int3");
  }
  ch->flags.bits.available = 1;
  Print("kfree success.");
}

void DumpHeap() {
  ChunkHead *iter = chunk_root;
  uint64_t size = 0;
  while(iter != NULL) {
    Print_int("addr : 0x", (uint64_t)iter->chunk, 16);
    Print_int("  size : 0x", iter->size, 16);
    Print_int("  available : ", iter->flags.bits.available, 10);
    size += iter->size;
    iter = iter->next;
  }
  Print_int("all size : 0x", size, 16);
}
