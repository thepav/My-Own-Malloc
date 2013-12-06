#include "my_malloc.h"


metadata_t* remove_head(metadata_t** freelist, int index){
		//printf("6 index: %d\n",index);

	metadata_t* head = freelist[index];
		//printf("7: %d\n",(head==NULL));

	freelist[index] = head->next;
		//printf("8\n");
	if(((metadata_t*)freelist[index])!=NULL){
		((metadata_t*)freelist[index])->prev = NULL;
	}
		//printf("9\n");

	head->next = NULL;
		//printf("10\n");

	head->prev = NULL;
			//printf("11\n");

	return head;
}


metadata_t* add_to_back(metadata_t** freelist,int index, metadata_t* node){
	metadata_t* head = freelist[index];
			//printf("20\n");

	if(head==NULL){
		freelist[index] = node;
		node->next = NULL;
		node->prev = NULL;
				//printf("21\n");
 
		return node;
	}else{
		metadata_t* n = head;
		while(n->next!=NULL){
			n = n->next;
		}
						//printf("22\n");

		n->next = node;
		node->prev = n;
		node->next =NULL;
						//printf("23\n");

		return node;
	}
}

metadata_t* removeNode(metadata_t** freelist, int index,metadata_t* node){
	if(node->prev==NULL && node->next==NULL){
		freelist[index]= NULL;
		return node;
	}else if(node->prev==NULL){
		freelist[index] = node->next;
		node->next->prev= NULL;
		return node;
	}else if(node->next==NULL){
		node->prev->next = NULL;
		return node;
	}else{
		node->prev->next= node->next;
		node->next->prev = node->prev;
		return node;
	}

}

void printListSizes(metadata_t** freelist){
printf("Sizes: ");

	for(int i =0;i<8;i++){
		int s =0;
		metadata_t* n = freelist[i];
		while(n){
			s++;
			n=n->next;
		}
		printf("%d:%d ",i,s);
	}
	printf("\n");
}
