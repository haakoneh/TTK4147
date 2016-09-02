#include <stdlib.h>
#include <stdio.h>

int global_variable = 0;

static void thread(void *data){
	int *local_variable = (int *) data;
	int i;
	for(i = 0; i<10; i++){
		global_variable++;
		*local_variable += 1;
	}

	pthread_exit(NULL);
}

int main(){
	pthread_t thread1, thread2;
	int local_variable = 0;
	
	pthread_create(&thread1,NULL,thread,&local_variable);
	pthread_create(&thread2,NULL,thread,&local_variable);
	
	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);
	
	printf("Local Variable: %i\n", local_variable);
	printf("Global Variable: %i\n", global_variable);

	return 0;
}

