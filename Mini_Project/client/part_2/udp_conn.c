#include "miniproject.h"
#include "udp_conn.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

#define IP_ADDRESS 						"192.168.0.1"
#define DOUBLE_TO_CHAR_LENGTH 	23  

struct udp_conn socket_conn;
pthread_mutex_t udp_send_mutex;

int send_with_mutex(char *data)
{
	int ret;
	
	pthread_mutex_lock(&udp_send_mutex);
	ret = udp_send(&socket_conn, data, strlen(data)+1);
	pthread_mutex_unlock(&udp_send_mutex);
	
	return ret;
}

int udp_init()
{	
	pthread_mutex_init(&udp_send_mutex, NULL);
	
	return udp_init_client(&socket_conn, 9999, IP_ADDRESS);
}

int send_start()
{
	char msg[] = "START";
	
	return send_with_mutex(msg);
}

int send_get()
{
	char msg[] = "GET";
	
	return send_with_mutex(msg);
}

int send_signal_ack()
{
	char msg[] = "SIGNAL_ACK";
	
	return send_with_mutex(msg);	
}

int send_stop()
{
	char msg[] = "STOP";
	
	return send_with_mutex(msg);	
}

int send_set(double u_value)
{
	char msg[DOUBLE_TO_CHAR_LENGTH + 5];
	sprintf(msg,"%s","SET:\0");

	char number[DOUBLE_TO_CHAR_LENGTH]; 
	sprintf(number, "%f", u_value);

	strcat(msg,number);
	
	return send_with_mutex(msg);	
}

int receive_data(char *buf)
{
	return udp_receive(&socket_conn, buf, BUFFER_SIZE);
}


