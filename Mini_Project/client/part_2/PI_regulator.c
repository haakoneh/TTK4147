#include "miniproject.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "udp_conn.h"
#include <semaphore.h>
#include <string.h>

#define KP 				10.0
#define KI 				800.0
#define REFERENCE 	1.0
#define MS 				1000.0
#define PERIOD_US 	3 * MS
#define PERIOD_S 		PERIOD_US/(1000.0 * MS)
#define BUFFER_SIZE 	256


sem_t regulator_sem;
sem_t signal_sem;

double integral = 0.0;
char regulator_buffer[BUFFER_SIZE];


void load_regulator_buffer(char *regulator_value){
	strcpy(regulator_buffer,regulator_value);
}

double parse_get(char buffer[]){
	int i = 0;
	int j = 0;
	
	char our_number[256] ;
	while(buffer[i++] != ':');

	return atof(buffer + i);
}
double regulator_calculation(double y){
	double error = REFERENCE - y;
	integral += error * PERIOD_S;
	double u = KP * error + KI * integral;

	return u;
}

void *receiver(){
	char recv_buffer[256];

	while(1){
		receive_get(recv_buffer);
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
	while(1){
		sem_wait(&signal_sem);
		send_signal_ack();
	}
}

void *regulator(){
	double y = 0.0;
	double u = 0.0;
	struct timespec time_start;
	//clock_gettime(CLOCK_REALTIME,&time_start); // 2
	
	//send_get();
	
	while(1){
		clock_gettime(CLOCK_REALTIME,&time_start); 
		send_get();
		sem_wait(&regulator_sem);
			
		y = parse_get(regulator_buffer);
		u = regulator_calculation(y);
		
		send_set(u);
		
		timespec_add_us(&time_start, PERIOD_US);	 
		clock_nanosleep_the_second(&time_start);   
		//send_get();
		
	}
}

int main(){
	pthread_t regulator_thread,signal_thread,receiver_thread;

	memset(regulator_buffer,'\0', sizeof(regulator_buffer));
	
	sem_init(&regulator_sem,0,0);
	sem_init(&signal_sem,0,0);
	
	struct timespec time_start;
	struct timespec time_stop;
		
	udp_init();
	pthread_create(&receiver_thread,NULL,receiver,0);
	pthread_create(&signal_thread,NULL,return_sig_ack,0);
	send_start();
	pthread_create(&regulator_thread,NULL,regulator,0);
	
	
	
	
	
	clock_gettime(CLOCK_REALTIME,&time_stop);
	timespec_add_us(&time_stop, 500*MS);
	clock_nanosleep_the_second(&time_stop);

	send_stop();
	return 0;
}
	
