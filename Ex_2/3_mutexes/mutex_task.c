#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int var1 = 0;
int var2 = 0;
int running = 1;
pthread_mutex_t lock;
static void thread1(void *data){
  int *thread_id = (int*)data;

  while(running){
    pthread_mutex_lock(&lock);
    var1 = var1 + 1;
    var2 = var1;
    pthread_mutex_unlock(&lock);
  }

  //pthread_exit(3);
}

static void thread2(void *data){
  int i = 1;
  for(i;i<20;i++){
    pthread_mutex_lock(&lock);
    printf("Number 1 is %d, number 2 is %d \n",var1,var2);
    usleep(10000);
    pthread_mutex_unlock(&lock);
  }
  running = 0;
}

int main(){

  pthread_mutex_init(&lock,NULL);

  pthread_t thread_1,thread_2;
  pthread_create(&thread_1,NULL,thread1,NULL);
  pthread_create(&thread_2,NULL,thread2,NULL);

  pthread_join(thread_2,NULL);
  return 0;
}
