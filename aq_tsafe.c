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

// We need to deviate from our sequential implementation,
// because of the pthread variables needs to be queue specific, not node specific
typedef struct Queue {
  queueNode *head;
  pthread_mutex_t lock;
  pthread_cond_t alarm_received;
  pthread_cond_t message_sent;
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

AlarmQueue aq_create( ) {
  Queue *aq = malloc(sizeof(Queue));
  if (aq != NULL) {
    aq->head = NULL;
    pthread_mutex_init(&(aq->lock), NULL);
    pthread_cond_init(&(aq->alarm_received), NULL);
    pthread_cond_init(&(aq->message_sent), NULL);
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

  pthread_mutex_lock(&queue->lock);


  queueNode *temp = queue->head;

  while (temp != NULL) {
    if (temp->msgKind == AQ_ALARM && k == AQ_ALARM) {
      pthread_cond_wait(&queue->message_sent, &queue->lock);
      insertAtEnd(&queue->head, msg, k);
      pthread_mutex_unlock(&queue->lock);
      return 0;
    }
    temp = temp->next;
  }
  insertAtEnd(&queue->head, msg, k);

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

  pthread_mutex_lock(&queue->lock);

  queueNode *temp = queue->head;


  // Check if any of the messages are of alarm kind
  while (temp != NULL) {

    if (temp->msgKind == AQ_ALARM) {

      int kind = deleteNode(&queue->head, *(int*)temp->msg, msg);
      pthread_cond_signal(&queue->alarm_received);
      pthread_mutex_unlock(&queue->lock);
      return kind;
    }
    temp = temp->next;
  }


  if (queue->head == NULL) {
      pthread_mutex_unlock(&queue->lock);
      return AQ_NO_MSG;
    }

  temp = queue->head;

  MsgKind kind = deleteNode(&queue->head, temp->msgKind, msg);
  pthread_mutex_unlock(&queue->lock);
  return kind;
}

int aq_size( AlarmQueue aq) {
  int size = 0;
  Queue *queue = aq;
  pthread_mutex_lock(&queue->lock);

  queueNode *temp = queue->head;

  while (temp != NULL) {
    size++;
    temp = temp->next;
  }
  pthread_mutex_unlock(&queue->lock);
  return size;
}

int aq_alarms(AlarmQueue aq) {
  int alarmCount = 0;
  Queue *queue = aq;
  pthread_mutex_lock(&queue->lock);

  queueNode *temp = queue->head;

  while (temp!= NULL) {
    if (temp->msgKind == AQ_ALARM) {
      alarmCount++;
    }
    temp = temp->next;
  }
  pthread_mutex_unlock(&queue->lock);
  return alarmCount;
}



