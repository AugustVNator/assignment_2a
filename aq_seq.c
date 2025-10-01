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

  queueNode head = *(queueNode*)aq;
  queueNode *temp = &head;
  if (head.msg == NULL) {
    head.msg = msg;
    head.K = k;
  }

  if (head.next == NULL) {
    head.next = malloc(sizeof(queueNode));

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
  int size = 0;
  queueNode *head = aq;
  queueNode *temp = head;

  while (temp->next != NULL) {
    size++;
    temp = temp->next;
  }
  return size;

}

int aq_alarms( AlarmQueue aq) {
  int alarmCount = 0;
  queueNode *head = aq;
  queueNode *temp = head;

  while (temp->next != NULL) {
    if (temp->K == AQ_ALARM) {
      alarmCount++;
    }

    temp = temp->next;
  }
  return alarmCount;
}



