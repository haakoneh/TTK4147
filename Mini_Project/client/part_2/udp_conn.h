#include "miniproject.h"

#define BUFFER_SIZE 	1472

int conn_init();
int send_start();
int send_get();
int send_set(float u_value);
int send_stop();
int receive_get(char *buf);
