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

queueNode *createQueueNode(void *msg, MsgKind k) {
  queueNode *newQueueNode = malloc(sizeof(queueNode));
  newQueueNode->msg = msg;
  newQueueNode->msgKind = k;
  newQueueNode->next = NULL;
  return newQueueNode;
}

void insertAtEnd(queueNode **head, void *msg, MsgKind k) {
  queueNode *newQueueNode = createQueueNode(msg, k);
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
  AlarmQueue **aq = malloc(sizeof(queueNode *));

  if (aq != NULL) {
    *aq = NULL; // Initialise head of queue to null

    /**
     * Header specifies return value
     * should be "handle to alarm queue".
     * It seems to be working 
     */
    return aq;
  }

  return NULL;
}

int aq_send( AlarmQueue aq, void * msg, MsgKind k){
  // Guard checks for queue and message
  if ( aq == NULL ) {
    return AQ_UNINIT;
  }
  if (msg == NULL) {
    return AQ_NULL_MSG;
  }


  queueNode **head = aq;
  queueNode *temp = *head;

  // Check if the alarm queue already contains alarm message
  while (temp != NULL) {
    if (temp->msgKind == AQ_ALARM && k == AQ_ALARM) {
      return AQ_NO_ROOM;
    }
    temp = temp->next;
  }

  insertAtEnd(head, msg, k);

  return 0;
}

int aq_recv( AlarmQueue aq, void * * msg) {
  if (msg == NULL) {
    return AQ_NO_MSG;
  }



  return AQ_NOT_IMPL;
}

int aq_size( AlarmQueue aq) {
  int size = 0;
  queueNode **head = aq;
  queueNode *temp = *head;

  while (temp != NULL) {
    size++;
    temp = temp->next;
  }
  return size;

}

int aq_alarms( AlarmQueue aq) {
  int alarmCount = 0;
  queueNode **head = aq;
  queueNode *temp = *head;

  while (temp!= NULL) {
    if (temp->msgKind == AQ_ALARM) {
      alarmCount++;
    }

    temp = temp->next;
  }
  return alarmCount;
}



