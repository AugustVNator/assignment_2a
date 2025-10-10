#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include "aq.h"

#include "aux.h"

static AlarmQueue queue;
void *producer1(void *arg) {
    int *alarm1 = malloc(sizeof(int)); //int to alarm in queue
    int *alarm2 = malloc(sizeof(int));
    *alarm1 = 1;
    *alarm2 = 2;
    printf("Producer1 sends alarm 1\n");
    aq_send(queue, alarm1, AQ_ALARM);
    printf("Producer1 tries to send alarm 2 but is blocked\n");
    aq_send(queue, alarm2, AQ_ALARM);
    printf("Queue is now unblocked so producer1 sends alarm 2\n");
    return NULL;
}

void *producer2(void *arg) {
    msleep(500);//waits to send normal message so the blocking of alarm2 can happen
    int *normal3 = malloc(sizeof(int));
    *normal3 = 3;
    printf("Producer2 sends normal message\n");
    aq_send(queue, normal3, AQ_NORMAL);

    msleep(1000);

    int *normal4 = malloc(sizeof(int));
    *normal4 = 4;
    printf("Producer2 sends second normal message\n");
    aq_send(queue, normal4, AQ_NORMAL);

    return NULL;
}

void *consumer(void *arg) {
    msleep(1000);//ensures blocking of alarm2 happens
    void *msg;
    aq_recv(queue, &msg);
    printf("Consumer receives alarm message %d\n", *(int*)msg);

    msleep(800);//it is unblocked now so doesnt need that much time in practice
    aq_recv(queue, &msg);
    printf("Consumer receives alarm message %d\n", *(int*)msg);

    msleep(500);//unblocked as well
    aq_recv(queue, &msg);
    printf("Consumer receives normal message\n");

    msleep(500);//unblocked as well
    aq_recv(queue, &msg);
    printf("Consumer receives second normal message\n");
    return NULL;
}

int main(void) {
    setbuf(stdout, NULL);
    pthread_t t1, t2, t3;

    queue = aq_create();
    if (!queue) {
        printf("Alarm queue could not be created\n");
        return 1;
    }

    pthread_create(&t1, NULL, producer1,  NULL);
    pthread_create(&t2, NULL, consumer,  NULL);
    pthread_create(&t3, NULL, producer2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    printf("Blocking/unblocking test is successful.\n");
    return 0;
}