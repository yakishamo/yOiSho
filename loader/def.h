#ifndef DEF_H
#define DEF_H

#include "types64.h"

typedef uint32_t size_t;

#define offsetof(s,m) (size_t)&(((s*)0)->m)

#endif /* DEF_H */
