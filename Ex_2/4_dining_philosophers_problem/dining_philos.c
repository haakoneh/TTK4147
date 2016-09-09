#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//this is the deadlock version

pthread_mutex_t forks[5];


void *philosopher(void* philo_id){
	int *id = (int*) philo_id;
	while(1){
		if(*id == 0){
			pthread_mutex_lock(&forks[0]);
			printf("Philosopher %d have taken fork %d \n", *id,*id);
			pthread_mutex_lock(&forks[4]);
			printf("Philosopher %d have taken fork %d \n", *id,4);

			printf("Philosopher %d is ready to eat now \n", *id);
			usleep(500); //eat for 1 sec
			printf("Philosopher %d has finished his turn\n",*id);
			pthread_mutex_unlock(&forks[0]);
			printf("Philosopher %d have released fork %d \n", *id,0);
			pthread_mutex_unlock(&forks[4]);
			printf("Philosopher %d have released fork %d \n", *id,4);

		}else if(*id == 4){
			pthread_mutex_lock(&forks[*id]);
			printf("Philosopher %d have taken fork %d \n", *id,*id);
			pthread_mutex_lock(&forks[*id-1]);
			printf("Philosopher %d have taken fork %d \n", *id,*id-1);
			printf("Philosopher %d is ready to eat now \n", *id);
			usleep(500); //eat for 1 sec
			printf("Philosopher %d has finished his turn\n",*id);
			pthread_mutex_unlock(&forks[*id]);
			printf("Philosopher %d have released fork %d \n", *id,*id);
			pthread_mutex_unlock(&forks[*id-1]);
			printf("Philosopher %d have released fork %d \n", *id,*id-1);
			
		}else {
			pthread_mutex_lock(&forks[*id]);
			printf("Philosopher %d have taken fork %d \n", *id,*id);
			pthread_mutex_lock(&forks[*id -1]);
			printf("Philosopher %d have taken fork %d \n", *id,*id-1);
			printf("Philosopher %d is ready to eat now \n", *id);
			usleep(500); //eat for 1 sec
			printf("Philosopher %d has finished his turn\n",*id);
			pthread_mutex_unlock(&forks[*id]);
			printf("Philosopher %d have released fork %d \n", *id,*id);
			pthread_mutex_unlock(&forks[*id -1]);
			printf("Philosopher %d have released fork %d \n", *id,*id-1);
		}
		
		sleep(1);
}


}



int main(){
	int i;
	for(i = 0; i<5;i++){
		pthread_mutex_init(&forks[i],NULL);
	}
	pthread_t thread1,thread2,thread3,thread4,thread5;
	int philosopher_id[5] = {0,1,2,3,4};
	pthread_create(&thread1,NULL,philosopher,&philosopher_id[0]);
	pthread_create(&thread2,NULL,philosopher,&philosopher_id[1]);
	pthread_create(&thread3,NULL,philosopher,&philosopher_id[2]);
	pthread_create(&thread4,NULL,philosopher,&philosopher_id[3]);
	pthread_create(&thread5,NULL,philosopher,&philosopher_id[4]);
	pthread_join(thread1,NULL);

	return 0;

}