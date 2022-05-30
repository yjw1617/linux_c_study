#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
struct taxi{
    struct taxi* next;
    int num;
};

pthread_cond_t hasTaxi = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
struct taxi* Head = NULL;
void* taxiTask(void* arg){
    pthread_detach(pthread_self());
    printf("taxi arrived thread\n");

    struct taxi* tx;
    int i = 1;
    while(1){
        tx = malloc(sizeof(struct taxi));
        tx->num = i++;
        printf("taxi %d comming\n", tx->num);
        pthread_mutex_lock(&lock);
        tx->next = Head;
        Head = tx;

        pthread_cond_signal(&hasTaxi);//给消费者传递消息

        pthread_mutex_unlock(&lock);
    
        sleep(5);

    }
    pthread_exit(0);
}

void* userTask(void* arg){
    pthread_detach(pthread_self());
    printf("take taxi thread\n");
    struct taxi* tx;
    while(1){
        pthread_mutex_lock(&lock);

        while(Head == NULL){
            pthread_cond_wait(&hasTaxi, &lock);//等待接受生产者的到来
        }
        tx = Head;
        Head = tx->next;
        printf("take taxi %d\n", tx->num);
        free(tx);
        pthread_mutex_unlock(&lock);
    }
    pthread_exit(0);
}
int main(){
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, taxiTask, NULL);
    pthread_create(&tid2, NULL, userTask, NULL);
    while(1){
        sleep(1);
    }
}