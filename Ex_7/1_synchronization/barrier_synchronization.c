#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <native/task.h>
#include <native/timer.h>
#include  <rtdk.h>
#include <native/sem.h>

RT_SEM the_truest_semaphore;
RT_TASK task_H;
RT_TASK task_M;
RT_TASK task_L;

void hungry_task(void *arg){
  int (*task_id) = (int*)arg;
  rt_sem_p(&the_truest_semaphore,TM_INFINITE);
  rt_printf("Task with priority %i received the semaphore\n",*task_id);

}

int main(){
  mlockall(MCL_CURRENT  | MCL_FUTURE);
  int task_id[3] = {1,2,3};
  rt_print_auto_init(1);
  rt_printf("1_synchronization\n");
  rt_sem_create(&the_truest_semaphore,"indeed the truest one",1,S_FIFO);
  rt_task_shadow(NULL,"High priority task",3, T_CPU(1) | T_JOINABLE);
  rt_task_create(&task_M,"Medium priority task", 50,2, T_CPU(1) | T_JOINABLE);
  rt_task_create(&task_L,"Low priority task", 50,1, T_CPU(1) | T_JOINABLE);


  rt_task_start(&task_M,&hungry_task,&task_id[1]);
  rt_task_start(&task_L,&hungry_task,&task_id[0]);

  rt_task_sleep(100*1000*1000);
  rt_sem_broadcast(&the_truest_semaphore);
  rt_task_sleep(100*1000*1000);
  rt_sem_delete(&the_truest_semaphore);
  return 0;
}
