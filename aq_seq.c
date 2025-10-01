/**
 * @file   aq.c
 * @Author 02335 team
 * @date   October, 2024
 * @brief  Alarm queue skeleton implementation
 */

#include <stdio.h>
#include <stdlib.h>

#include "aq.h"

typedef struct queueNode{
  void *msg;
  MsgKind msgKind;
  struct queueNode *next;
} queueNode;

queueNode *createQueueNode(void *msg) {
  queueNode *newQueueNode = malloc(sizeof(queueNode));
  newQueueNode->msg = msg;
  newQueueNode->next = NULL;
  return newQueueNode;
}

void insertAtEnd(queueNode **head, void *msg) {
  queueNode *newQueueNode = createQueueNode(msg);
  if (*head == NULL) {
    *head = newQueueNode;
    return;
  }
  queueNode *temp = *head;
  while (temp->next != NULL) {
    temp = temp->next;
  }
  temp->next = newQueueNode;
}

AlarmQueue aq_create( ) {

  queueNode *head = malloc(sizeof(queueNode));
  AlarmQueue aq = &head;

  if (aq != NULL) {
    return aq;
  }
  return NULL;
}

int aq_send( AlarmQueue aq, void * msg, MsgKind k){
  queueNode head = *(queueNode*)aq;
  queueNode *temp = &head;

  // Check if the alarm queue already contains alarm message
  while (temp->next != NULL) {
    printf("hello\n");
    if (temp->msgKind == AQ_ALARM) {
      return AQ_NO_ROOM;
    }
    temp = temp->next;
  }
  insertAtEnd(&head, msg);


  return k;
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
    if (temp->msgKind == AQ_ALARM) {
      alarmCount++;
    }

    temp = temp->next;
  }
  return alarmCount;
}



