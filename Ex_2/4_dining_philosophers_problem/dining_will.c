#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

//Working withot deadlock

pthread_mutex_t forks[5];
//pthread_mutex_t waiter;
sem_t philosopherGoEat[5];
pthread_mutex_t fork_status;
bool fork_available[5];

bool canEat(bool fork1_available, bool fork2_available){
	if(fork1_available ==true && fork2_available == true){
		fork1_available = false;
		fork2_available = false;
		return true;
	}
	return false;
}

void *waiter(){
	int i;
	while(1){
		for(i = 0;i<5;i++){
			bool philCanEat = false;
			
			pthread_mutex_lock(&fork_status);
			if(i == 0){	philCanEat = canEat(fork_available[0], fork_available[4]); 		} 
			else { 		philCanEat = canEat(fork_available[i], fork_available[i - 1]);  }
			pthread_mutex_unlock(&fork_status);
			
			if(philCanEat){
				sem_post(&philosopherGoEat[i]);
			}		
		}
		usleep(1000*100); //Waiter waiting
	}
}

void *philosopher(void* philo_id){
	int *id = (int*) philo_id;
	while(1){
		if(*id == 0){
			sem_wait(&philosopherGoEat[*id]);
			pthread_mutex_lock(&forks[0]);
			pthread_mutex_lock(&forks[4]);
			printf("Philosopher %d is ready to eat now \n", *id);

			sleep(1); //Eating	
	
			pthread_mutex_unlock(&forks[4]);
			pthread_mutex_unlock(&forks[0]);
			pthread_mutex_lock(&fork_status);
			fork_available[0] = true;
			fork_available[4] = true;
			pthread_mutex_unlock(&fork_status);
			printf("Philosopher %d has finished his turn\n",*id);

		}else {
			sem_wait(&philosopherGoEat[*id]);
			pthread_mutex_lock(&forks[*id]);
			pthread_mutex_lock(&forks[*id -1]);
			printf("Philosopher %d is ready to eat now \n", *id);

			sleep(1); //Eating	

			pthread_mutex_unlock(&forks[*id-1]);
			pthread_mutex_unlock(&forks[*id]);
			pthread_mutex_lock(&fork_status);
			fork_available[*id-1] = true;
			fork_available[*id] = true;
			pthread_mutex_unlock(&fork_status);
			printf("Philosopher %d has finished his turn\n",*id);
		}

		usleep(1000*100); //Thinking 
	}
}

int main(){
	int i;

	pthread_mutex_init(&fork_status,NULL);
	for(i = 0; i<5;i++){
		pthread_mutex_init(&forks[i],NULL);
		sem_init(&philosopherGoEat[i],0,1);
		fork_available[i] = true;
	}
	pthread_t thread1,thread2,thread3,thread4,thread5,thread6;
	int philosopher_id[5] = {0,1,2,3,4};
	pthread_create(&thread6,NULL,waiter,NULL);
	pthread_create(&thread1,NULL,philosopher,&philosopher_id[0]);
	pthread_create(&thread2,NULL,philosopher,&philosopher_id[1]);
	pthread_create(&thread3,NULL,philosopher,&philosopher_id[2]);
	pthread_create(&thread4,NULL,philosopher,&philosopher_id[3]);
	pthread_create(&thread5,NULL,philosopher,&philosopher_id[4]);
	pthread_join(thread1,NULL);

	return 0;
}
