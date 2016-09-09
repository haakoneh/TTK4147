#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
sem_t semaphore;
static void thread(void *data){
  int *thread_id = (int*)data;

  while(1){
    sem_wait(&semaphore);
    printf("Hello from thread: %d \n", *thread_id);
    sleep(1);
    sem_post(&semaphore);

  }

  //pthread_exit(3);
}


int main(){

  sem_init(&semaphore,0,1);

  pthread_t thread1,thread2,thread3,thread4,thread5;
  int thread_id[5] = {1,2,3,4,5};
  pthread_create(&thread1,NULL,thread,&thread_id[0]);
  pthread_create(&thread2,NULL,thread,&thread_id[1]);
  pthread_create(&thread3,NULL,thread,&thread_id[2]);
  pthread_create(&thread4,NULL,thread,&thread_id[3]);
  pthread_create(&thread5,NULL,thread,&thread_id[4]);
  pthread_join(thread1,NULL);


  return 0;
}
