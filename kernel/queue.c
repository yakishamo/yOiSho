#include "queue.h"
#include "../common/types64.h"

static QUEUE_ITEM *GetQueueEnd(QUEUE *queue) {
  if(queue == NULL) return NULL;
  if(queue->item == NULL) return NULL;
  QUEUE_ITEM *item = queue->item;
  while(item->next != NULL) {
    item = item->next;
  }
  return item;
}

QUEUE *PushQueue(QUEUE *queue, QUEUE_ITEM *new) {
  if(queue == NULL) return NULL;
  if(queue->item == NULL) {
    queue->item = new;
    queue->item->next = NULL;
  } else {
    QUEUE_ITEM *item = queue->item;
    while(item->next != NULL) 
      item = item->next;
    item->next = new;
    item->next->next = NULL;
  }
  return queue;
}

QUEUE_ITEM *PopQueue(QUEUE *queue) {
  if(queue == NULL) return NULL;
  if(queue->item->next == NULL) {
    QUEUE_ITEM *ret = queue->item;
    queue->item = NULL;
    return ret;
  } else {
    QUEUE_ITEM *item = queue->item;
    while(item->next->next != NULL) {
      item = item->next;
    }
    QUEUE_ITEM *ret = item->next;
    item->next = NULL;
    return ret;
  }
  return NULL;
}

uint64_t GetQueueSize(QUEUE *queue) {
  uint64_t size = 0;
  if(queue == NULL) return 0;
  if(queue->item == NULL) return 0;
  size++;
  QUEUE_ITEM *item = queue->item;
  while(item->next != NULL) {
    size++;
    item = item->next;
  }
  return size;
}
