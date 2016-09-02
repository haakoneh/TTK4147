#include <stdlib.h>
#include <stdio.h>

//example with use of fork

//http://unix.stackexchange.com/questions/5337/whats-the-difference-between-fork-and-vfork

// vfork used to be an optimalization of fork, as it did not copy the process image
// Fork makes an identical copy of the parent process, with new memory space, while
// Vfork shares the memory space.


int global_variable = 0;

int main(){
	int local_variable = 0;
	vfork();
	int i =0;
	for(i; i<10;i++){
		local_variable++;
		global_variable++;
	}
	
	printf("Local Variable: %i\n", local_variable);
	printf("Global Variable: %i\n", global_variable);

	return 0;
		
}

//vfork will start a process that increments the local and global variable, which is shared
//with the parent process. After incrementing 10 times, the process will terminate,deleting
// the local variable. Thats why we receive a weird number. The global variable does not
// get deleted by the child process.
