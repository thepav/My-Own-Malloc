#ifndef __LIST_H__
#define __LIST_H__

/* we need this *ONLY* for the size_t typedef */
#include <stdio.h>
#include "my_malloc.h"


metadata_t* remove_head(metadata_t** freelist, int index);
metadata_t* add_to_back(metadata_t** freelist,int index, metadata_t* node);
void printListSizes(metadata_t** freelist);
metadata_t* removeNode(metadata_t** freelist, int index,metadata_t* node);



#endif /* __LIST_H__ */
