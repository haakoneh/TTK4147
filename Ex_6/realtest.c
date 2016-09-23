#include <stdio.h>  
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include <native/task.h>  
#include <native/timer.h>

#include  <rtdk.h>

#include "io.h"

#define WAIT_TIME 10000

/*

2.1



*/


static void *disturbance_thread(void *data){
	//set_cpu(1);
	int a;
	while(1){
		a = 2 + 2;
	}
	pthread_exit(NULL);
}

void a_receiver(void *data){		
	rt_task_set_periodic(NULL, TM_NOW, WAIT_TIME);

	while(1){
		if(io_read(1) == 0){
			io_write(1,0);
			rt_task_sleep(1);
			//usleep(5);
			io_write(1,1);
		}
		
		rt_task_wait_period(NULL);	
		
	}
}

void b_receiver(void *data){
	//set_cpu(1);
	rt_task_set_periodic(NULL, TM_NOW, WAIT_TIME);
	while(1){
		if(io_read(2) == 0){
			io_write(2,0);
			rt_task_sleep(1);
			//usleep(5);
			io_write(2,1);
		}
		rt_task_wait_period(NULL);
	}
}

	//set_cpu(1);
void c_receiver(void *data){
	rt_task_set_periodic(NULL, TM_NOW, WAIT_TIME);
	while(1){
		if(io_read(3) == 0){
			io_write(3,0);
			rt_task_sleep(1);
			//usleep(5);
			io_write(3,1);
		}
		rt_task_wait_period(NULL);
	}
}

int main(){
	mlockall(MCL_CURRENT|MCL_FUTURE);
	rt_print_auto_init(1);
	rt_printf("fuck this shit");
	pthread_t disturbance_threads[10];
	io_init();
	int i;
	for(i = 0; i<10;i++){
		pthread_create(&disturbance_threads[i],NULL,disturbance_thread,0);
	}

	
	RT_TASK task_a;
	RT_TASK task_b;
	RT_TASK task_c;
	

	rt_task_create(&task_a,"Response A task", 0, 99, T_CPU(0));//T_CPU(0));
	rt_task_start(&task_a,&a_receiver, NULL);
	rt_task_create(&task_b,"Response b task", 0, 99, T_CPU(0));
	rt_task_create(&task_c,"Response c task", 0, 99, T_CPU(0));
	
	rt_task_start(&task_b,&b_receiver,NULL);
	rt_task_start(&task_c,&c_receiver,NULL);
	while(1);
	
		
	
	return 0;

}
