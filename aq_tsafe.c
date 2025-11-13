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


typedef struct queueNode{
  void *msg;
  MsgKind msgKind;
  struct queueNode *next;
} queueNode;

typedef struct Queue {
  queueNode *head;
  pthread_mutex_t lock;
  pthread_cond_t cond;
} Queue;

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
      MsgKind retval = temp->msgKind;
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

int testSetLock (Queue *queue) {
  int initialValueOfLock = (int) &queue->lock ;
  printf("%d \n", initialValueOfLock);
  pthread_mutex_lock(&queue->lock);
  return initialValueOfLock;
}

AlarmQueue aq_create( ) {
  Queue *aq = malloc(sizeof(Queue *));
  if (aq != NULL) {
    aq->head = NULL;
    pthread_mutex_init(&(aq->lock), NULL);
    pthread_cond_init(&(aq->cond), NULL);
    return aq;
  }
  return NULL;
}

int aq_send( AlarmQueue aq, void * msg, MsgKind k){
  if (aq == NULL ) {
    return AQ_UNINIT;
  }
  if (msg == NULL) {
    return AQ_NULL_MSG;
  }
  Queue *queue = aq;

  while (testSetLock(queue) == 1) {}

  queueNode **head = &queue->head;
  queueNode *temp = *head;

  while (temp != NULL) {
    if (temp->msgKind == AQ_ALARM && k == AQ_ALARM) {
      return AQ_NO_ROOM;
    }
    temp = temp->next;
  }
  insertAtEnd(head, msg, k);


  pthread_mutex_unlock(&queue->lock);
  return 0;
}

int aq_recv(AlarmQueue aq, void * * msg) {
    if (msg == NULL) {
    return AQ_NULL_MSG;
  }
  if (aq == NULL) {
    return AQ_UNINIT;
  }

  Queue *queue = aq;
  while (testSetLock(queue) == 1) {}

  queueNode **head = &queue->head;

  while (*head == NULL) {
    pthread_cond_wait(&notEmpty, &lock);
  }

  queueNode *temp = *head;

  while (temp != NULL) {
    if (temp->msgKind == AQ_ALARM) {
      int kind = deleteNode(head, *(int*)temp->msg, msg);
      pthread_cond_signal(&alarmFree);
      pthread_mutex_unlock(&lock);
      return kind;
    }
    temp = temp->next;
  }

  temp = *head;
  MsgKind kind = deleteNode(head, temp->msgKind, msg);
  pthread_mutex_unlock(&lock);
  return kind;
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

int aq_alarms(AlarmQueue aq) {
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



