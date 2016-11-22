#include "miniproject.h"

#define BUFFER_SIZE 	64

int conn_init();
int send_start();
int send_get();
int send_set(double u_value);
int send_stop();
int receive_get(char *buf);
