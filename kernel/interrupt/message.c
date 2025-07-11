#include "message.h"

IMQueue *IMQueueInit(IMQueue *queue, IM *buf, uint32_t size) {
  queue->size = size;
  queue->buf = buf;
  queue->head = 0;
  queue->tail = 0;
  return queue;
}

IMQueue *IMQueuePush(IMQueue *queue, IM *message) {
  if((queue->head+1)%(queue->size) == queue->tail) {
    return queue;
  }
  queue->head = (queue->head + 1) % queue->size;
  queue->buf[queue->head] = *message;
  return queue;
}

IM IMQueuePop(IMQueue *queue) {
  if(queue->head == queue->tail) {
    IM m = {kEmpty};
    return m;
  }
  IM message = queue->buf[queue->tail];
  queue->buf[queue->tail].type = kEmpty;
  queue->tail = (queue->tail + 1) % queue->size;
  return message;
}
