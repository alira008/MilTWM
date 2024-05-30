#include "channel.h"
#include "queue.h"

Channel *channel_new(size_t capacity) {
  Channel *channel = malloc(sizeof(Channel));
  if (!channel) {
    return NULL;
  }

  InitializeSRWLock(&channel->lock);
  channel->queue = queue_new(capacity);

  if (!channel->queue) {
    free(channel->queue);
    free(channel);
    return NULL;
  }

  return channel;
}

void channel_free(Channel *channel) {
  queue_free(channel->queue);
  free(channel);
}

void channel_close(Channel *channel) {
  AcquireSRWLockExclusive(&channel->lock);
  if (channel->closed) {
    return;
  } else {
    channel->closed = true;
  }
  ReleaseSRWLockExclusive(&channel->lock);
}

bool channel_is_closed(Channel *channel) {
  AcquireSRWLockExclusive(&channel->lock);
  bool closed = channel->closed;
  ReleaseSRWLockExclusive(&channel->lock);
  return closed;
}

bool channel_send(Channel *channel, void *data) {
  if (channel_is_closed(channel)) {
    return false;
  }

  AcquireSRWLockExclusive(&channel->lock);
  queue_add(channel->queue, data);
  ReleaseSRWLockExclusive(&channel->lock);

  return true;
}

bool channel_try_receive(Channel *channel, void *data) {
  if (TryAcquireSRWLockShared(&channel->lock)) {
    bool is_removed = false;
    data = queue_remove(channel->queue);
    if (data != NULL) {
      is_removed = true;
    }
    ReleaseSRWLockShared(&channel->lock);
    return is_removed;
  }
  return false;
}

size_t channel_size(Channel *channel) { return channel->queue->size; }
