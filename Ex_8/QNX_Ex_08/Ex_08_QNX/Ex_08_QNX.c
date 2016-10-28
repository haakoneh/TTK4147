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

typedef struct pid_data{
	pthread_mutex_t pid_mutex;
	pid_t pid;
}pid_data;




int main(int argc, char *argv[]) {
	pthread_mutexattr_t myattr;
	pid_data *pid_mutex_ex;
	//pid_mutex_ex = malloc(sizeof(pid_data));

	int file_descriptor = shm_open("/sharedpid",O_RDWR | O_CREAT,S_IRWXU);

	ftruncate(file_descriptor,sizeof(pid_data));

	void *s_mem_ptr = mmap(0,sizeof(pid_data),PROT_READ | PROT_WRITE,MAP_SHARED,file_descriptor,0);

	//(struct pid_data*) s_mem_ptr;
	pid_mutex_ex = (pid_data* )s_mem_ptr;
	//s_mem_ptr = (struct pid_data*) &pid_mutex_ex;


	pthread_mutexattr_init(&myattr);
	pthread_mutexattr_setpshared(&myattr,PTHREAD_PROCESS_SHARED);

	pthread_mutex_init(&pid_mutex_ex->pid_mutex,&myattr);


	pid_mutex_ex->pid = getpid();

	printf("the pid is: %i", getpid());
	return EXIT_SUCCESS;
	//return 0;
}
