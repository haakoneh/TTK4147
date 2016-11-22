#define BUFFER_SIZE 	1492

int udp_init();
int send_start();
int send_get();
int send_signal_ack();
int send_stop();
int send_set(double u_value);
int receive_data(char *buf);
