/**
 * @file   aq.c
 * @Author 02335 team
 * @date   October, 2024
 * @brief  Alarm queue skeleton implementation
 */

#include "aq.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  notEmpty  = PTHREAD_COND_INITIALIZER;
static pthread_cond_t  alarmFree = PTHREAD_COND_INITIALIZER;

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

int deleteNode(queueNode **head, int msgContent, void **msg) {
  queueNode **tempP = head;
  queueNode *temp = *head;
  while (temp != NULL) {
    if (*(int*)temp->msg == msgContent) {
      int retval = *(int*)temp->msg;
      *msg = temp->msg;
      *tempP = temp->next;
      free (temp);
      return retval;
    }
    tempP = &temp->next;
    temp = temp->next;
  }
  return -1;
}

AlarmQueue aq_create( ) {
  AlarmQueue **aq = malloc(sizeof(queueNode *));

  if (aq != NULL) {
    *aq = NULL;
    return aq;
  }
  return NULL;
}

int aq_send( AlarmQueue aq, void * msg, MsgKind k){
  if ( aq == NULL ) {
    return AQ_UNINIT;
  }
  if (msg == NULL) {
    return AQ_NULL_MSG;
  }

  pthread_mutex_lock(&lock);
  queueNode **head = aq;

  while (*head == NULL) {
    pthread_cond_wait(&notEmpty, &lock);
  }

  queueNode *temp = *head;

  while (temp != NULL) {
    if (temp->msgKind == AQ_ALARM && k == AQ_ALARM) {
      pthread_mutex_unlock(&lock);
      return AQ_NO_ROOM;
    }
    temp = temp->next;
  }

  insertAtEnd(head, msg, k);
  pthread_mutex_unlock(&lock);
  return 0;
}

int aq_recv(AlarmQueue aq, void * * msg) {
  if (msg == NULL) {
    return AQ_NULL_MSG;
  }
  pthread_mutex_lock(&lock);
  queueNode **head = aq;

  while (*head == NULL) {
    pthread_cond_wait(&notEmpty, &lock);
  }

  queueNode *temp = *head;

  while (temp != NULL) {
    if (temp->msgKind == AQ_ALARM) {
      pthread_mutex_unlock(&lock);
     return deleteNode(head, *(int*)temp->msg, msg);
    }
    temp = temp->next;
  }
  pthread_mutex_unlock(&lock);
  return deleteNode(head, *(int*)temp->msg, msg);
}

int aq_size( AlarmQueue aq) {
  int size = 0;

  pthread_mutex_lock(&lock);
  queueNode **head = aq;
  queueNode *temp = *head;

  while (temp != NULL) {
    size++;
    temp = temp->next;
  }
  pthread_mutex_unlock(&lock);
  return size;
}

int aq_alarms( AlarmQueue aq) {
  int alarmCount = 0;
  pthread_mutex_lock(&lock);
  queueNode **head = aq;
  queueNode *temp = *head;

  while (temp!= NULL) {
    if (temp->msgKind == AQ_ALARM) {
      alarmCount++;
    }

    temp = temp->next;
  }
  pthread_mutex_unlock(&lock);
  return alarmCount;
}



