#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/times.h>
static void *sleep_thread(void *data){
	int *int_ptr = (int *) data;
	
	//while(1){
		printf("hello from thread %i \n",*int_ptr);
		sleep(5);
	//}

	pthread_exit(NULL);
	
	
}

void busy_wait_delay(int seconds){
	int i, dummy;
	int tps = sysconf(_SC_CLK_TCK);
	clock_t start;
	struct tms exec_time;
	times(&exec_time);
	start = exec_time.tms_utime;
	while( (exec_time.tms_utime - start) < (seconds *tps) ){
		for(i=0; i<1000; i++){
			dummy = i;
		}
		times(&exec_time);
	}
}

static void *busy_wait_thread(void *data){
	int *int_ptr = (int *) data;
	
	//while(1){
		printf("hello from thread %i \n",*int_ptr);
		busy_wait_delay(5);
	//}

	pthread_exit(NULL);
	
	
}





int main(void){
	pthread_t thread;
	int a = 1;
	int b = 2;
	//pthread_create(&thread,NULL,sleep_thread,&a);
	//pthread_create(&thread,NULL,sleep_thread,&b);
	pthread_create(&thread,NULL,busy_wait_thread,&a);
	pthread_create(&thread,NULL,busy_wait_thread,&b);
	
	pthread_join(thread,NULL);
	return 0;
}
