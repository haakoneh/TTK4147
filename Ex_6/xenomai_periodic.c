 #include <stdio.h>  
 #include <signal.h>
 #include <unistd.h>
 #include <sys/mman.h>
 #include <native/task.h>  
#include <native/timer.h>
 #include  <rtdk.h>


 #include "io.h"
/*

2.1



*/

void a_receiver(void *data){
	//set_cpu(1);
	while(1){
		if(io_read(1) == 0){
			io_write(1,0);
			usleep(5);
			io_write(1,1);
		}
	}
}

void b_receiver(void *data){
	//set_cpu(1);
	while(1){
		if(io_read(2) == 0){
			io_write(2,0);
			usleep(5);
			io_write(2,1);
		}
	}
}

void c_receiver(void *data){
	//set_cpu(1);
	while(1){
		if(io_read(3) == 0){
			io_write(3,0);
			usleep(5);
			io_write(3,1);
		}
	}
}

int main(){
	/*RT_TASK task_a;
	RT_TASK task_b;
	RT_TASK task_c;
	rt_task_create(&task_a,"Response A task", 0, 99, T_JOINABLE );
	rt_task_start(&task_a,&a_receiver, NULL);*/
	 while(1);
	
	return 0;

}
