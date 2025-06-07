#ifndef QUEUE_H
#define QUEUE_H

#include "../common/types64.h"

typedef struct QUEUE QUEUE;
typedef struct QUEUE_ITEM QUEUE_ITEM;

QUEUE *PushQueue(QUEUE *queue, QUEUE_ITEM *new);
QUEUE_ITEM *PopQueue(QUEUE *queue);
uint64_t GetQueueSize(QUEUE *queue);

#endif /* QUEUE_H */
