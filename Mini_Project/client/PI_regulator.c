#include "miniproject.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "udp_conn.h"
#define KP 10.0
#define KI 800.0
#define REFERENCE 1.0
#define MS 1000.0
#define PERIOD_US 4* MS
#define PERIOD_S PERIOD_US/(1000.0 * MS)
double integral = 0.0;

double parseGet(char buffer[]){
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
int main(){
	struct timespec time_start;
	struct timespec time_stop;
	clock_gettime(CLOCK_REALTIME,&time_start);
	clock_gettime(CLOCK_REALTIME,&time_stop);
	timespec_add_us(&time_stop, 500*MS);
	double y = 0.0;
	double u = 0.0;
	char buffer[256];
	socket_init();
	send_start();
	while(time_stop.tv_nsec > time_start.tv_nsec){
		send_get();

		receive_get(buffer);
		
		y = parseGet(buffer);
		//time start <--
		u = regulator_calculation(y);
		send_set(u);
		timespec_add_us(&time_start, PERIOD_US);
		clock_nanosleep(&time_start);
		
	}
	send_stop();
	return 0;

}
	
