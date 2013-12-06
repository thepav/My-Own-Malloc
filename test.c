#include "my_malloc.h"
#include <stdio.h>
#include <stdlib.h>
typedef struct lol{
	int i;
	int j;
	char hi;
} lolcano;

int main() {
	printf("\n\n\n\n\n-------\n");

	/*
	 * Test code goes here
	 */
	 printf("SIZEOF metadata_t: %d\n", sizeof(metadata_t));

	void* l= my_malloc(96);
	
	for(int i =0;i<96;i++){
		*(((char*)l)+i)=0xF;
	}

	if(l==NULL){
		printf("HELLO\n");
	}else{
		printf("%d\n",*((int*)l));
	}
	 
	return 0;
}
