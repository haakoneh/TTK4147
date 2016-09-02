#include <stdlib.h>
#include <stdio.h>

//1a

int allocate(int value){
	int *ptr = NULL;
	if((ptr = malloc(100000 * sizeof(int))) == NULL){
		perror("NULL ERROR");
		return -1;
	
	}
	*ptr = value;
	printf("test of allocated memory: %i\n ", *ptr);
	return 1;
}

int main(){
	int i = 0;
	while(allocate(i) != -1){
		i++;
	}
	return 0;
}



void nothing(){
	//do nothing
}
