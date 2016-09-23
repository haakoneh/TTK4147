#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/times.h>

#include "io.h"


static void *thread_a(void *data){
	while(1){
		if(io_read(1) == 0){
			io_write(1,0);
			usleep(5);
			io_write(1,1);
		}
	}	
	
	pthread_exit(NULL);
}
static void *thread_b(void *data){
	while(1){
		if(io_read(2) == 0){
			io_write(2,0);
			usleep(5);
			io_write(2,1);
		}
	}	
	pthread_exit(NULL);
}
static void *thread_c(void *data){
	while(1){
		if(io_read(3) == 0){
			io_write(3,0);
			usleep(5);
			io_write(3,1);
		}
	}	
	pthread_exit(NULL);
}



int main1(){
	
	io_init();
	
	pthread_t thread1,thread2,thread3;

	pthread_create(&thread1,NULL,thread_a,0);
	pthread_create(&thread2,NULL,thread_b,0);
	pthread_create(&thread3,NULL,thread_c,0);
	pthread_join(thread1,NULL);
	return 0;
}
