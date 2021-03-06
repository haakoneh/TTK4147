#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/times.h>

#include "../io.h"
#include "sched.h"

int set_cpu(int cpu_number){
	// setting cpu set to the selected cpu
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);
	// set cpu set to current thread and return
	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t),
	&cpu);

}

static void *disturbance_thread(void *data){
	
	set_cpu(1);
	int a;	
	while(1){
		a = 2 + 2;
	}

}


static void *thread_a(void *data){
	set_cpu(1);
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
	set_cpu(1);
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
	set_cpu(1);
	while(1){
		if(io_read(3) == 0){
			io_write(3,0);
			usleep(5);
			io_write(3,1);
		}
	}	
	pthread_exit(NULL);
}



int main(){
	io_init();
	
	pthread_t thread1,thread2,thread3;

	pthread_t disturbance_threads[10];
	int i;
	for(i = 0; i<10;i++){	
		pthread_create(&disturbance_threads[i],NULL,disturbance_thread,0);

	}

	pthread_create(&thread1,NULL,thread_a,0);
	pthread_create(&thread2,NULL,thread_b,0);
	pthread_create(&thread3,NULL,thread_c,0);
	pthread_join(thread1,NULL);
	return 0;


}
