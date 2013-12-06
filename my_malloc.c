#include "my_malloc.h"
#include "list.h"
/* You *MUST* use this macro when calling my_sbrk to allocate the 
 * appropriate size. Failure to do so may result in an incorrect
 * grading!
 */
#define SBRK_SIZE 2048

/* If you want to use debugging printouts, it is HIGHLY recommended
 * to use this macro or something similar. If you produce output from
 * your code then you will receive a 20 point deduction. You have been
 * warned.
 */
#ifdef DEBUG
#define DEBUG_PRINT(x) printf x
#else
#define DEBUG_PRINT(x)
#endif


/* make sure this always points to the beginning of your current
 * heap space! if it does not, then the grader will not be able
 * to run correctly and you will receive 0 credit. remember that
 * only the _first_ call to my_malloc() returns the beginning of
 * the heap. sequential calls will return a pointer to the newly
 * added space!
 * Technically this should be declared static because we do not
 * want any program outside of this file to be able to access it
 * however, DO NOT CHANGE the way this variable is declared or
 * it will break the autograder.
 */
void* heap;

/* our freelist structure - this is where the current freelist of
 * blocks will be maintained. failure to maintain the list inside
 * of this structure will result in no credit, as the grader will
 * expect it to be maintained here.
 * Technically this should be declared static for the same reasons
 * as above, but DO NOT CHANGE the way this structure is declared
 * or it will break the autograder.
 */
metadata_t* freelist[8];
/**** SIZES FOR THE FREE LIST ****
 * freelist[0] -> 16
 * freelist[1] -> 32
 * freelist[2] -> 64
 * freelist[3] -> 128
 * freelist[4] -> 256
 * freelist[5] -> 512
 * freelist[6] -> 1024
 * freelist[7] -> 2048
 */
 void breakBlock(int biggerIndex,int smallerIndex);
int getIndex(int size){
	if(size==16){
		return 0;
	}else if(size==32){
		return 1;
	}else if(size==64){
		return 2;
	}else if(size==128){
		return 3;
	}else if(size==256){
		return 4;
	}else if(size==512){
		return 5;
	}else if(size==1024){
		return 6;
	}else{
		return 7;
	}
}

void print_list(metadata_t* list[8])
{
	int i;
	for (i = 0; i < 8; i++)
	{
		fprintf(stderr, "%s\n", "---------------");
		fprintf(stderr, "Printing list %d:\n", i);
		if (list[i])
		{
			metadata_t *current = list[i];
			while(current)
			{
				fprintf(stderr, "\taddress: %d, size: %d, next: %p, prev: %p\n", (int) current, current->size, (void *) current->next, (void *) current->prev);
				current = current->next;
			}
		}
		else
		{
			fprintf(stderr, "%s\n", "(null)");
		}
	}
}

void* my_malloc(size_t size)
{
	//Check to see if they asked for more than 2048 bytes
	double s = (double)(size+sizeof(metadata_t));

	if(s>2048){
		ERRNO=SINGLE_REQUEST_TOO_LARGE;
		return NULL;
	}

	//Set up the heap if this is the first run of my_malloc
	if(!heap){
		for(int i =0;i<8;i++){
			freelist[i]=NULL;
		}
		heap = my_sbrk(SBRK_SIZE);
		if(heap==(void*)-1){
			// TODO set error code 
			return NULL;
		}

		metadata_t* newBlock = (metadata_t*)heap;
		newBlock->in_use=0;
		newBlock->size=SBRK_SIZE;
		newBlock->next = NULL;
		newBlock->prev = NULL;
		freelist[7] = newBlock;
	}

	//Check to see if all memory is full
	int allFull = 1;
	for(int i =0;i<8;i++){
		if(freelist[i]!=NULL){
			allFull=0;
		}
	}
	if(allFull){
		ERRNO=OUT_OF_MEMORY;
		return NULL;
	}

	int index = 0;
	//figure out the size of the block to satisfy
	if(s!= 16&&s!= 32&&s!= 64&&s!= 128&&s!= 256&&s!= 512&&s!= 1024&&s!= 2048){
		index = ((int)(fastlog2(s)+1))-4;
	}else{
		index = ((int)(fastlog2(s)))-4;
	}

	if(freelist[index]!=NULL){
		//there is a block of this size free
		metadata_t* block = remove_head(freelist,index);
		block->in_use = 1;
		return (block+1);
	}else{
		//there is not a block directly of the size we need. Lets see 
		//if there are bigger ones
		int thereIs = -1;
		for(int i = index+1;i<8;i++){
			if(freelist[i]!=NULL&&thereIs<0){
				thereIs= i;
			}
		}
		if(thereIs>=0){

			breakBlock(thereIs,index); // break the bigger block down to the size we need
		}else{
			void* point = my_sbrk(SBRK_SIZE);
			if(point==(void*)-1){
				// TODO set error code 
				return NULL;
			}
			metadata_t* newBlock = (metadata_t*)point;
			newBlock->in_use=0;
			newBlock->size=SBRK_SIZE;
			newBlock->next = NULL;
			newBlock->prev = NULL;
			freelist[7] = newBlock;
			breakBlock(7,index); //break the new block of 2048 bytes that 
							 //was added to the size that we need			
		} 
		metadata_t* block = remove_head(freelist,index);
		block->in_use = 1;
		ERRNO= NO_ERROR;
		return (void*)(((char*)block)+sizeof(metadata_t));

	}

}
void breakBlock(int biggerIndex,int smallerIndex){
	for(int i =biggerIndex;i>smallerIndex;i--){
		metadata_t* big = remove_head(freelist,i);
		big->size/=2;

		// Create new half
		metadata_t *big2 = (metadata_t *)((char *)big + big->size);
		big2->size = big->size;
		big2->in_use = 0;
		big2->next = NULL;
		big2->prev = big;

		add_to_back(freelist,i-1,big);
		add_to_back(freelist,i-1,big2);
	}
}

void* my_calloc(size_t num, size_t size)
{	
	//Check to see if the request was going to be too much for the 8kb heap
	if(num*size>8192){
		ERRNO=SINGLE_REQUEST_TOO_LARGE; //because single request is supposed to trump out of memory
		return NULL;
	}
	void* point=my_malloc(num*size); // Just assuming that the total size is less than 2kb

	//initializing all the data to 0s
	for(int i =0;i<num*size;i++){
		((char*)point)[i] = 0;
	}

	return point;
}
void my_free(void* ptr)
{
  /* FIX ME */
	metadata_t* meta = (metadata_t*)(((char*)ptr)-sizeof(metadata_t));
	while(1){
		print_list(freelist);
		int size = (int)meta->size;
		meta->in_use=0;
		//If there are no blocks free of that size then just put it in the free list
		if(freelist[getIndex(size)]==NULL){
			add_to_back(freelist,getIndex(size),meta);
			return;
			printf("1\n");
		}else{
			printf("2\n");
			int n = getIndex((int)meta->size)+4;
			metadata_t* buddy = (metadata_t*)(((int)((int)meta-(int)heap))^((int)meta->size));
						printf("3: n:%d p:%p\n",n,(void*)buddy);
			int temp = 1<<n;
			int bit = ((int)buddy&(int)temp)>>n;
			//Get buddy
			buddy = (metadata_t*)((int)buddy+(int)heap);
			if(meta->size==buddy->size && buddy->in_use==0 ){
								printf("4\n");
				if(bit){
					//buddy is on the right side
					meta->size*=2;
					removeNode(freelist,getIndex(size),meta);
					removeNode(freelist,getIndex(size),buddy);
					add_to_back(freelist,getIndex(size)+1,meta);
				}else{
					//buddy is on the left side
					meta = buddy;
					meta->size*=2;
					removeNode(freelist,getIndex(size),meta);
					removeNode(freelist,getIndex(size),buddy);
					add_to_back(freelist,getIndex(size)+1,meta);
				}
			}else{
				add_to_back(freelist,getIndex(size),meta);
				return;	
			}

		}
	}
}

void* my_memmove(void* dest, const void* src, size_t num_bytes)
{
  //Get a buffer to store the data to the moved
  char* buffer = my_malloc(num_bytes);
  for(int i =0;i<num_bytes;i++){
  	buffer[i]=((char*)src)[i];
  }
  for(int i =0;i<num_bytes;i++){
  	((char*)dest)[i] = buffer[i];
  }
  return dest;
}
