#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <native/task.h>
#include <native/timer.h>
#include  <rtdk.h>
#include <native/mutex.h>

#define TIME_UNIT 10 //ms
#define MUTEX 1
void busy_wait_ms(unsigned long delay);
void print_pri(RT_TASK *task, char *s);
int rt_task_sleep_ms(unsigned long delay);

RT_MUTEX mutex_a;
RT_MUTEX mutex_b;

RT_TASK task_H;
RT_TASK task_L;

void low_p_function(void *arg){
    print_pri(NULL,"low_p_function is about to aquire mutex a\n");
    rt_mutex_acquire(&mutex_a,TM_INFINITE);
    print_pri(NULL,"low_p_function has aquired the mutex a\n");

    busy_wait_ms(3*TIME_UNIT);

    print_pri(NULL,"low_p_function is about to aquire mutex b\n");
    rt_mutex_acquire(&mutex_b,TM_INFINITE);
    print_pri(NULL,"low_p function has acquired mutex b\n");

    busy_wait_ms(3*TIME_UNIT);

    print_pri(NULL,"low_p function is about to release mutex b\n");
    rt_mutex_release(&mutex_b);

    print_pri(NULL,"low_p function is about to release mutex a\n");
    rt_mutex_release(&mutex_a);

    busy_wait_ms(TIME_UNIT);
}

void high_p_function(void *arg){
    rt_task_sleep_ms(TIME_UNIT);

    print_pri(NULL,"high_p_function wants to acquire mutex b\n");
    rt_mutex_acquire(&mutex_b,TM_INFINITE);
    print_pri(NULL,"high_p_function has acquired mutex b\n");

    busy_wait_ms(TIME_UNIT);

    print_pri(NULL,"high_p_function wants to acquire mutex a\n");
    rt_mutex_acquire(&mutex_a,TM_INFINITE);
    print_pri(NULL,"high_p_function has acquired mutex a\n");

    busy_wait_ms(2*TIME_UNIT);

    print_pri(NULL,"high_p_function is about to release mutex a\n");
    rt_mutex_release(&mutex_a);

    print_pri(NULL,"high_p_function is about to release mutex b\n");
    rt_mutex_release(&mutex_b);

    busy_wait_ms(TIME_UNIT);
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

    rt_mutex_create(&mutex_a,"mutex_a");
    rt_mutex_create(&mutex_b,"mutex_b");

    rt_task_create(&task_H,"High priority task",50,3,T_CPU(1) | T_JOINABLE);
    rt_task_create(&task_L,"Low priority task", 50,1, T_CPU(1) | T_JOINABLE);

    rt_task_start(&task_H,&high_p_function,&task_id[2]);
    rt_task_start(&task_L,&low_p_function,&task_id[0]);

    while(1);

    return 0;
}
