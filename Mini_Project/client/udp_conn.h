#include "miniproject.h"

#define IP_ADDRESS "192.168.0.1"




int conn_init();

int send_start();
int send_get();
int send_set();
int sernd_stop();

int receive_get(char *buf);
