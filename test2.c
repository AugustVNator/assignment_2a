#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include "aq.h"

#include "aux.h"

static AlarmQueue queue;
void * producerNormal(void *arg) {
  int *normal = malloc(sizeof(int));
  *normal=1;//int to alarm in queue
  printf("ProducerNormal sends normal message\n");
  aq_send(queue,normal, AQ_NORMAL);

  return NULL;}

void * producerAlarm (void *arg) {
    msleep(500); //comes slower than normal to show queue priority
    int *alarm = malloc(sizeof(int));
    *alarm=3;
    printf("ProducerAlarm sends alarm message\n");
    aq_send(queue,alarm,AQ_ALARM);
    return NULL;
}

void * consumer(void *arg) {
  void * msg;
  msleep(800); //waits for both messages to have been sent
  aq_recv(queue,&msg);
  printf("Consumer receives alarm message\n");
  aq_recv(queue,&msg);
  printf("Consumer receives normal message\n");

  return NULL;}

int main(void) {
  setbuf(stdout, NULL);
  pthread_t t1,t2,t3;

  queue = aq_create();
  if (!queue) {
    printf("Alarm queue could not be created\n");
    return 1;
  }
  pthread_create(&t1,NULL,producerNormal,NULL);
  pthread_create(&t2,NULL,producerAlarm,NULL);
  pthread_create(&t3,NULL,consumer,NULL);
  pthread_join(t1,NULL);
  pthread_join(t2,NULL);
  pthread_join(t3,NULL);
  printf("Alarm priority test is successful\n");
  return 0;





}
