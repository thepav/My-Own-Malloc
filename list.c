//Pavleen Thukral

#include "my_malloc.h"


metadata_t* remove_head(metadata_t** freelist, int index){
	metadata_t* head = freelist[index];
	//really simple
	freelist[index] = head->next;
	if(((metadata_t*)freelist[index])!=NULL){
		((metadata_t*)freelist[index])->prev = NULL;
	}
	head->next = NULL;
	head->prev = NULL;
	return head;
}


metadata_t* add_to_back(metadata_t** freelist,int index, metadata_t* node){
	metadata_t* head = freelist[index];
	// If the list is empty
	if(head==NULL){
		freelist[index] = node;
		node->next = NULL;
		node->prev = NULL;
		return node;
	}else{
	// If the list is not empty
		metadata_t* n = head;
		while(n->next!=NULL){
			n = n->next;
		}
		n->next = node;
		node->prev = n;
		node->next =NULL;
		return node;
	}
}

//Remove a particular node from a linked list in the freelist
metadata_t* removeNode(metadata_t** freelist, int index,metadata_t* node){
	
	if(node->prev==NULL && node->next==NULL){
		//When this node is the only node in the linked list 
		freelist[index]= NULL;
		return node;
	}else if(node->prev==NULL){
		//When this node is the first node in the linked list
		freelist[index] = node->next;
		node->next->prev= NULL;
		return node;
	}else if(node->next==NULL){
		//When this node is the last node in the linked list
		node->prev->next = NULL;
		return node;
	}else{
		node->prev->next= node->next;
		node->next->prev = node->prev;
		return node;
	}

}

//Simply prints the size of each linked list in the freelist
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
