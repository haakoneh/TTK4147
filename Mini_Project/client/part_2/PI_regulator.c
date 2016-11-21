#include "miniproject.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "udp_conn.h"
#include <semaphore.h>
#include <string.h>

#define BASEPERIOD_US 		1
#define PERIOD_MS			1000 * BASEPERIOD_US
#define PERIOD_S 			1000 * PERIOD_MS

#define KP 					10.0
#define KI 					800.0
#define REFERENCE 			1.0
#define REG_PERIOD_US 		4019
#define REG_PERIOD_S 		REG_PERIOD_US/(PERIOD_S) 	

#define SIG_PERIOD_US 		257

#define SERVER_DATA_OFFSET 	8

sem_t regulator_sem;
sem_t signal_sem;

float integral = 0.0;
char regulator_buffer[BUFFER_SIZE];


void load_regulator_buffer(char *regulator_value){
	strcpy(regulator_buffer,regulator_value);
}

float parse_get(char buffer[]){
	return atof(buffer + SERVER_DATA_OFFSET); 
}

float regulator_calculation(float y){
	float error = REFERENCE - y;
	integral += error * REG_PERIOD_S;
	float u = KP * error + KI * integral;

	return u;
}

void *receiver(){
	char recv_buffer[BUFFER_SIZE];

	while(1){
		receive_data(recv_buffer);
		if(recv_buffer[0] == 'G'){
			load_regulator_buffer(recv_buffer);
			sem_post(&regulator_sem);
		}else if(recv_buffer[0] == 'S'){
			sem_post(&signal_sem);
		} else {
			// Do nothing
		}
	}
}

void *return_sig_ack(){

	struct timespec time_start;
	clock_gettime(CLOCK_REALTIME, &time_start);
	timespec_add_us(&time_start, SIG_PERIOD);
	
	while(1){
		sem_wait(&signal_sem);
		send_signal_ack();
			 
		/* FRAGILE: if an iteration takes more that 1 sec, nanosleep will..  throw an error? Return immediately? Untested */
		clock_nanosleep_the_second(&time_start);
		clock_gettime(CLOCK_REALTIME,&time_start); 
		timespec_add_us(&time_start, SIG_PERIOD);
	}
}

void *regulator(){
	float y = 0.0;
	float u = 0.0;
	
	struct timespec time_start;
	clock_gettime(CLOCK_REALTIME,&time_start);
	timespec_add_us(&time_start, REG_PERIOD); 
	
	send_start();

	while(1){
		send_get();
		
		sem_wait(&regulator_sem);
			
		y = parse_get(regulator_buffer);
		u = regulator_calculation(y);
		
		send_set(u);
		
		/* FRAGILE: if an iteration takes more that 1 sec, nanosleep will..  throw an error? Return immediately? Untested */	 
		clock_nanosleep_the_second(&time_start);
		clock_gettime(CLOCK_REALTIME,&time_start);
		timespec_add_us(&time_start, REG_PERIOD);    
	}
}

int main(){
	pthread_t 	regulator_thread,
				signal_thread,
				receiver_thread;

	memset(regulator_buffer,'\0', sizeof(regulator_buffer));
	
	sem_init(&regulator_sem,0,0);
	sem_init(&signal_sem,0,0);
		
	udp_init();
	
	pthread_create(&receiver_thread,NULL,receiver,0);
	pthread_create(&signal_thread,NULL,return_sig_ack,0);
	pthread_create(&regulator_thread,NULL,regulator,0);
	
	struct timespec time_stop;
	clock_gettime(CLOCK_REALTIME,&time_stop);
	timespec_add_us(&time_stop, 500*MS);
	clock_nanosleep_the_second(&time_stop);

	send_stop();
	
	return 0;
}
	
