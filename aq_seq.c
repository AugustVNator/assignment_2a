/**
 * @file   aq.c
 * @Author 02335 team
 * @date   October, 2024
 * @brief  Alarm queue skeleton implementation
 */

#include <stdlib.h>

#include "aq.h"

typedef struct {
  void *msg;
  MsgKind K;
  struct queueNode *next;
} queueNode;

AlarmQueue aq_create( ) {

  queueNode *head = malloc(sizeof(queueNode));
  AlarmQueue aq = &head;

  if (aq != NULL) {
    return aq;
  }
  return NULL;
}

int aq_send( AlarmQueue aq, void * msg, MsgKind k){
  if (k == AQ_ALARM /*&& aq->msg == AQ_ALARM*/) {
    return AQ_NO_ROOM;
  }

  return AQ_NOT_IMPL;
}

int aq_recv( AlarmQueue aq, void * * msg) {
  if (msg == NULL) {
    return AQ_NO_MSG;
  }
  return AQ_NOT_IMPL;
}

int aq_size( AlarmQueue aq) {
  return 0;
}

int aq_alarms( AlarmQueue aq) {
  return 0;
}



