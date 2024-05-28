#pragma once
#include "queue.h"
#include <Windows.h>
#include <stdbool.h>

typedef struct Channel {
  Queue *queue;
  bool closed;
  SRWLOCK lock;
} Channel;

Channel *channel_new(size_t capacity);
void channel_free(Channel *channel);
void channel_close(Channel *channel);
bool channel_is_closed(Channel *channel);
bool channel_send(Channel *channel, void *data);
bool channel_try_receive(Channel *channel, void *data);
size_t channel_size(Channel *channel);
