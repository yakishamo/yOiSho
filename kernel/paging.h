#ifndef PAGING_H
#define PAGING_H

#include "../common/types64.h"

typedef union pte pte_t;
typedef pte_t pd_t;
typedef pte_t pdp_t;
typedef pte_t pml4_t;

union pte {
  uint64_t data;
  struct {
    uint64_t present : 1;
    uint64_t read_write : 1;
    uint64_t user_accessable : 1;
    uint64_t write_through : 1;
    uint64_t cache_disable : 1;
    uint64_t accessed : 1;
    uint64_t dirty : 1;
    uint64_t huge_page : 1;
    uint64_t global : 1;
    uint64_t available_a : 3;
    uint64_t phys_addr : 40;
    uint64_t available_b : 11;
    uint64_t no_execute : 1;
  } __attribute__((packed)) bits;
} __attribute__((packed));

void SetupIdentityPaging();

#endif /* PAGING_H */
