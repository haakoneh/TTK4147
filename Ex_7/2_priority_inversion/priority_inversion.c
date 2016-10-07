#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <native/task.h>
#include <native/timer.h>
#include  <rtdk.h>
#include <native/sem.h>
#include <native/mutex.h>

#define TIME_UNIT 10 //ms
//#define MUTEX 1
void busy_wait_ms(unsigned long delay);
void print_pri(RT_TASK *task, char *s);
int rt_task_sleep_ms(unsigned long delay);



RT_SEM the_truest_semaphore;
RT_MUTEX the_truest_mutex;

RT_TASK task_H;
RT_TASK task_M;
RT_TASK task_L;

void low_p_function(void *arg){
  //acquire the semaphore

  #ifdef MUTEX
    print_pri(NULL,"low_p_function is about to aquire mutex\n");
    rt_mutex_acquire(&the_truest_mutex,TM_INFINITE);
    print_pri(NULL,"low_p_function has aquired the mutex\n");
    busy_wait_ms(3*TIME_UNIT);
    print_pri(NULL,"low_p function has released its ,mutex\n");
    rt_mutex_release(&the_truest_mutex);

  #else
    print_pri(NULL,"low_p_function is about to aquire semaphore\n");
    rt_sem_p(&the_truest_semaphore,TM_INFINITE);
    print_pri(NULL,"low_p_function has aquired the semaphore\n");
    busy_wait_ms(3*TIME_UNIT);
    print_pri(NULL,"low_p function has released its semaphore\n");
    rt_sem_v(&the_truest_semaphore);
  #endif
}

void medium_p_function(void*arg){
  rt_task_sleep_ms(TIME_UNIT);
  print_pri(NULL,"medium_p_function has awaken.\n");
  busy_wait_ms(5*TIME_UNIT);
  print_pri(NULL,"medium_p_functin has finished its run.\n");


}

void high_p_function(void *arg){
  #ifdef MUTEX
  rt_task_sleep_ms(2 * TIME_UNIT);
  print_pri(NULL,"high_p_function wants to start and tries to aquire the mutex\n");
  rt_mutex_acquire(&the_truest_mutex,TM_INFINITE);
  print_pri(NULL,"high_p_function has achieved the mutex\n");

  busy_wait_ms(2 * TIME_UNIT);
  rt_mutex_release(&the_truest_mutex);
  #else
    rt_task_sleep_ms(2 * TIME_UNIT);
    print_pri(NULL,"high_p_function wants to start and tries to aquire the semaphore\n");
    rt_sem_p(&the_truest_semaphore,TM_INFINITE);
    print_pri(NULL,"high_p_function has achieved the semaphore\n");

    busy_wait_ms(2 * TIME_UNIT);
    rt_sem_v(&the_truest_semaphore);
  #endif
}

void busy_wait_ms(unsigned long delay){
  unsigned long count = 0;
  while (count <= delay*10){
      rt_timer_spin(1000*100);
      count++;
  }
}

void print_pri(RT_TASK *task,char *s){
  struct rt_task_info temp;
  rt_task_inquire(task,&temp);
  rt_printf("b:%i c:%i \n",temp.bprio,temp.cprio);
  rt_printf(s);
}

int rt_task_sleep_ms(unsigned long delay){
    return rt_task_sleep(delay*1000*1000);
}



int main(){
  mlockall(MCL_CURRENT  | MCL_FUTURE);
  int task_id[3] = {1,2,3};
  rt_print_auto_init(1);
  #ifdef MUTEX
    rt_mutex_create(&the_truest_mutex,"indeed the truest one");
  #else
    rt_sem_create(&the_truest_semaphore,"indeed the truest one",1,S_FIFO);
  #endif



  //rt_task_shadow(NULL,"High priority task",3, T_CPU(1) | T_JOINABLE);
  rt_task_create(&task_H,"High priority task",50,3,T_CPU(1) | T_JOINABLE);
  rt_task_create(&task_M,"Medium priority task", 50,2, T_CPU(1) | T_JOINABLE);
  rt_task_create(&task_L,"Low priority task", 50,1, T_CPU(1) | T_JOINABLE);

  rt_task_start(&task_H,&high_p_function,&task_id[2]);
  rt_task_start(&task_M,&medium_p_function,&task_id[1]);
  rt_task_start(&task_L,&low_p_function,&task_id[0]);

  //rt_task_sleep_ms(100);
  //rt_sem_broadcast(&the_truest_semaphore);
  //rt_task_sleep_ms(100);
  //rt_sem_delete(&the_truest_semaphore);
  return 0;
}
