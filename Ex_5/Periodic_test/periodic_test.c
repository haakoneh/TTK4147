#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/times.h>

#include "../io.h"
#include "sched.h"

#define WAIT_TIME 100

void timespec_add_us(struct timespec *t,long us){
	//add microseconds to timespecs nanosecond counter
	t->tv_nsec += us*1000;

	// if wrapping nanosecond counter, increment second counter
	if(t->tv_nsec > 1000000000){
		t->tv_nsec = t->tv_nsec - 1000000000;
		t->tv_sec +=1;
	}
}

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
	struct timespec next;
	clock_gettime(CLOCK_REALTIME,&next);
	while(1){
		if(io_read(1) == 0){
			io_write(1,0);
			usleep(5);
			io_write(1,1);
		}
		
		timespec_add_us(&next, WAIT_TIME);
		clock_nanosleep(CLOCK_REALTIME,TIMER_ABSTIME,&next,NULL);
	}	
	pthread_exit(NULL);
}
static void *thread_b(void *data){
	set_cpu(1);
	struct timespec next;
	clock_gettime(CLOCK_REALTIME,&next);
	while(1){
		if(io_read(2) == 0){
			io_write(2,0);
			usleep(5);
			io_write(2,1);
		}
		timespec_add_us(&next, WAIT_TIME);
		clock_nanosleep(CLOCK_REALTIME,TIMER_ABSTIME,&next,NULL);
	}	
	pthread_exit(NULL);
}
static void *thread_c(void *data){
	set_cpu(1);
	struct timespec next;
	clock_gettime(CLOCK_REALTIME,&next);
	while(1){
		if(io_read(3) == 0){
			io_write(3,0);
			usleep(5);
			io_write(3,1);
		}	
		timespec_add_us(&next, WAIT_TIME);
		clock_nanosleep(CLOCK_REALTIME,TIMER_ABSTIME,&next,NULL);
	}	
	pthread_exit(NULL);
}

void test(){
	struct timespec next;
	clock_gettime(CLOCK_REALTIME,&next);
	while(1){
		io_write(1,1);
		timespec_add_us(&next, WAIT_TIME);
		clock_nanosleep(CLOCK_REALTIME,TIMER_ABSTIME,&next,NULL);
		io_write(1,0);
		timespec_add_us(&next, WAIT_TIME);
		clock_nanosleep(CLOCK_REALTIME,TIMER_ABSTIME,&next,NULL);
	}
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
