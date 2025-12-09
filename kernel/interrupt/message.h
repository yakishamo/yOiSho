#ifndef INTERRUPT_MESSAGE_H
#define INTERRUPT_MESSAGE_H

#include "../keyboard.h"
#include "../../common/types64.h"

#define IMQUEUE_MAX 128

// IM : InterruptMessage
typedef struct IMQueue IMQueue;
typedef struct IM IM;
typedef enum IMType IMType;

enum IMType {
  kEmpty,
  kInterruptKeyboard,
};

// KeyboardMessage
typedef struct KeyboardMessage KeyboardMessage;
struct KeyboardMessage {
  KeyCode code;
};

struct IMQueue {
  uint32_t size;
  uint32_t head;
  uint32_t tail;
  IM *buf;
};

struct IM {
  IMType type;
  union {
    KeyboardMessage keyboard_message;
  } field;
};

IMQueue *IMQueueInit(IMQueue *queue, IM *buf, uint32_t size);
IMQueue *IMQueuePush(IMQueue *queue, IM *message);
IM IMQueuePop(IMQueue *queue);
uint32_t IMQueueGetCurrentSize(IMQueue *queue);

#endif /* INTERRUPT_MESSAGE_H*/
