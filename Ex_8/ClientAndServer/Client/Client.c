#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/dispatch.h>
#include <sys/mman.h>
#include <pthread.h>
#include <process.h>

#define BUF_SIZE 256
typedef struct message{
	char msg;
	int number;
}message_buffer;

typedef struct pid_data{
	pthread_mutex_t pid_mutex;
	pid_t pid;
}pid_data;

pid_data sharedMemoryRead(){
	pid_data *other_struct;
	int file_descriptor = shm_open("/sharedpid",O_RDWR,S_IRWXU);
	void *s_mem_ptr = mmap(0,sizeof(pid_data),PROT_READ | PROT_WRITE,MAP_SHARED,file_descriptor,0);

	other_struct = (pid_data* )s_mem_ptr;
	printf("Pid is: %i", other_struct->pid);

	return *other_struct;
}

int set_priority(int priority){
	int policy;
	struct sched_param param;
	//check priority in range
	if(priority < 1 || priority > 63) return -1;
	pthread_getschedparam(pthread_self(),&policy,&param);
	param.sched_priority = priority;
	return pthread_setschedparam(pthread_self(),policy,&param);
}

int get_priority(){
	int policy;
	struct sched_param param;
	//get priority
	pthread_getschedparam(pthread_self(),&policy,&param);
	return param.sched_curpriority;
}

void *client_1(){
	set_priority(30);

	printf("Client 1 start\n");
	char receive_buf[BUF_SIZE];
	char send_buf[BUF_SIZE];
	memset(receive_buf,'\0',sizeof(receive_buf));
	memset(send_buf,'\0',sizeof(send_buf));
	strcpy(send_buf,"Hello from client 1!");

	pid_data pid_client = sharedMemoryRead();
	//Connect to a server/channel

	int channel_id = ConnectAttach(0,pid_client.pid, 1, 0, 0);

	//while(1){
		int status_value = MsgSend(channel_id, send_buf, BUF_SIZE,receive_buf,BUF_SIZE);
		printf(receive_buf);
	//	usleep(500 * 1000);
//	}
}

void *client_2(){
	set_priority(40);
	printf("Client 2 start\n");
	char receive_buf[BUF_SIZE];
		char send_buf[BUF_SIZE];
		memset(receive_buf,'\0',sizeof(receive_buf));
		memset(send_buf,'\0',sizeof(send_buf));
		strcpy(send_buf,"Hello from client 2!");

		pid_data pid_client = sharedMemoryRead();
		//Connect to a server/channel

		int channel_id = ConnectAttach(0,pid_client.pid, 1, 0, 0);

//		while(1){
			int status_value = MsgSend(channel_id, send_buf, BUF_SIZE,receive_buf,BUF_SIZE);
			printf(receive_buf);
	//		usleep(500 * 1000);
	//	}
}

void *client_3(){
	set_priority(50);
	printf("Client 3 start\n");
	char receive_buf[BUF_SIZE];
		char send_buf[BUF_SIZE];
		memset(receive_buf,'\0',sizeof(receive_buf));
		memset(send_buf,'\0',sizeof(send_buf));
		strcpy(send_buf,"Hello from client 3!");

		pid_data pid_client = sharedMemoryRead();
		//Connect to a server/channel

		int channel_id = ConnectAttach(0,pid_client.pid, 1, 0, 0);

	//	while(1){
			int status_value = MsgSend(channel_id, send_buf, BUF_SIZE,receive_buf,BUF_SIZE);
			printf(receive_buf);
//			usleep(500 * 1000);
	//	}
}

void *client_4(){
	set_priority(60);
	printf("Client 4 start\n");
	char receive_buf[BUF_SIZE];
		char send_buf[BUF_SIZE];
		memset(receive_buf,'\0',sizeof(receive_buf));
		memset(send_buf,'\0',sizeof(send_buf));

		strcpy(send_buf,"Hello from client 4!");
		pid_data pid_client = sharedMemoryRead();
		//Connect to a server/channel

		int channel_id = ConnectAttach(0,pid_client.pid, 1, 0, 0);

		//while(1){
			int status_value = MsgSend(channel_id, send_buf, BUF_SIZE,receive_buf,BUF_SIZE);
			printf(receive_buf);
		//	usleep(1000 * 1000);
		//}
}


int main(int argc, char *argv[]) {
	set_priority(61);

	printf("Client");
	/*message_buffer sent_data;
	message_buffer received_data;
	pid_data pid_client = sharedMemoryRead();
	//Connect to a server/channel*/

	//channel_id = ConnectAttach(0,pid_client.pid, 1, 0, 0);

	/*test */
	printf("Client 4 start\n");
	message_buffer sent_data;
	sent_data.msg = 'b';
	message_buffer received_data;
	pid_data pid_client = sharedMemoryRead();
	//Connect to a server/channel

	int channel_id = ConnectAttach(0,pid_client.pid, 1, 0, 0);

	/*end test*/



/*
	strcpy(send_buf,"Hello good sir server! Hope you received this! Please inform if so!");


	while(1){
		MsgSend(channel_id,send_buf, BUF_SIZE,receive_buf,BUF_SIZE);
		printf("We received some data %s \n",receive_buf);
		usleep(1000*500);

	}*/

	pthread_t thread1,thread2,thread3,thread4;
	pthread_create(&thread1,NULL,client_1,0);
	pthread_create(&thread2,NULL,client_2,0);
	pthread_create(&thread3,NULL,client_3,0);
	pthread_create(&thread4,NULL,client_4,0);
	pthread_join(thread1,NULL);
	ConnectDetach(channel_id);

	return EXIT_SUCCESS;
}
