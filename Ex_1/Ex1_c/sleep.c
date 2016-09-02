#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/times.h>
static void *sleep_thread(void *data){
	int *int_ptr = (int *) data;

	printf("hello from thread %i \n",*int_ptr);
	sleep(5);

	pthread_exit(NULL);	
}


int main(void){
	pthread_t thread;
	int a = 1;
	int b = 2;
	pthread_create(&thread,NULL,sleep_thread,&a);
	pthread_create(&thread,NULL,sleep_thread,&b);
	
	pthread_join(thread,NULL);
	return 0;
}
