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
static uint64_t tail_memory_size = KERNEL_HEAP_FRAMES*0x1000;

union ChunkFlag {
  uint64_t data;
  struct {
    uint64_t available : 1;
    uint64_t reserved : 63;
  } bits __attribute__((packed));
} __attribute__((packed));

struct ChunkHead {
  char magic[16];
  ChunkHead *back;
  ChunkHead *front;
  uint64_t size;
  ChunkFlag flags;
  uint8_t chunk[];
} __attribute__((packed));

static void InitChunk(ChunkHead *ch, ChunkHead *back, uint64_t size) {
  strcpy(ch->magic, KMALLOC_CHUNK_HEAD);
  ch->back = back;
  ch->front = NULL;
  ch->size = size;
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
  tail_memory_size -= sizeof(ChunkHead); 
  InitChunk(chunk_root, NULL, tail_memory_size);
  chunk_root->flags.bits.available = 1;
}

void *kmalloc(uint64_t size) {
  ChunkHead *iter = chunk_root;
  ChunkHead *iter_back = NULL;
  for(;;) {
    if(iter->flags.bits.available == 1 && iter->size >= size) {

