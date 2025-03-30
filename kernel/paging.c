#include "../common/types64.h"
#include "paging.h"
#include "asmfunc.h"
#include "frame.h"

const uint64_t PT_SIZE = 512;
const uint64_t PAGESIZE_4K = 4096;
const uint64_t PAGESIZE_2M = PAGESIZE_4K * PT_SIZE;
const uint64_t PAGESIZE_1G = PAGESIZE_2M * PT_SIZE;

pml4_t pml4[512]  __attribute__((__aligned__(4096)));
pdp_t pdp[512]    __attribute__((__aligned__(4096)));
pd_t pd[512][512] __attribute__((__aligned__(4096)));

void SetupIdentityPaging() {
  pml4[0].data = (uint64_t)&pdp[0] | 0x003;
  for(int i = 0; i < PT_SIZE; i++) {
    pdp[i].data = (uint64_t)&pd[i] | 0x003;
    for(int j = 0; j < PT_SIZE; j++) {
      pd[i][j].data = i * PAGESIZE_1G + j * PAGESIZE_2M | 0x083;
    }
  }
  WriteInteger("pml4 : 0x", (uint64_t)pml4, 16, 0, 0, &red);
  // while(1) asm("hlt");
  SetCR3((uint64_t)&pml4[0]);
}
