#include "my_malloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include "list.h"

// Chintan Parikh

extern metadata_t* freelist[8];

/* Methods for testing */
int count(metadata_t* head)
{
	int i = 0;
	while(head)
	{
		i++;
		head = head->next;
	}
	return i;
}

void assert_freelist_count(int args, ...)
{
	va_list valist;
	va_start(valist, args);
	int i;
	for (i = 7; i >=0; i--)
	{
		int j = va_arg(valist, int);
		assert(count(freelist[i]) == j);
	}
	va_end(valist);
}
void print_block(metadata_t *block) 
{
  fprintf(stderr, "Printing block data\n");
  fprintf(stderr, "address: %p\n", (void *) block);
  fprintf(stderr, "in use: %d\n", block->in_use);
  fprintf(stderr, "size: %d\n", block->size);
  fprintf(stderr, "next: %p\n", (void *) block->next);
  fprintf(stderr, "prev: %p\n", (void *) block->prev);
  fprintf(stderr, "\n");
}

void print_node(metadata_t* current)
{
	fprintf(stderr, "binary: %p, address: %p, size: %d, next: %p, prev: %p, in_use: %d\n", (void *) current, (void *) current, current->size, (void *) current->next, (void *) current->prev, (int) current->in_use);
}

// void print_list(metadata_t* list[8])
// {
// 	int i;
// 	for (i = 0; i < 8; i++)
// 	{
// 		fprintf(stderr, "%s\n", "---------------");
// 		fprintf(stderr, "Printing list %d:\n", i);
// 		if (list[i])
// 		{
// 			metadata_t *current = list[i];
// 			while(current)
// 			{
// 				current = current->next;
// 			}
// 		}
// 		else
// 		{
// 			fprintf(stderr, "%s\n", "(null)");
// 		}
// 	}
// }

int zeroed_out(char *first, int size) 
{
	int i;
    for (i = 0; i < size; i++) 
    {
        if (*(first + i) != 0)
        {
        	return 0;
        }
    }
    return 1;
}

void print_heap(metadata_t* list[8])
{
	fprintf(stderr, "%d\t | %d\t | %d\t | %d\t | %d\t | %d\t | %d\t | %d\t |\n", 2048, 1024, 512, 256, 128, 64, 32, 16);
	int count[8], i;

	for (i = 7; i >= 0; i--)
	{
		count[i] = 0;
		if (list[i])
		{
			metadata_t *head = list[i];
			while(head)
			{
				count[i]++;
				head = head->next;
			}
		}
		fprintf(stderr, "%d\t | ", count[i]);
	}

	fprintf(stderr, "\n");
}


int main() {
	// Test my_malloc, my_free normal conditions
	int *a = my_malloc(4);
	*a = 5;
	assert(*a = 5);
	assert_freelist_count(8, 0, 1, 1, 1, 1, 1, 1, 1);
	int *b = my_malloc(490);
	*b = 1;
	assert_freelist_count(8, 0, 1, 0, 1, 1, 1, 1, 1);
	int *c = my_malloc(490);
	*c = 1;
	assert_freelist_count(8, 0, 0, 1, 1, 1, 1, 1, 1);
	int *d = my_malloc(200);
	assert(ERRNO == NO_ERROR);
	*d = 1;
	assert_freelist_count(8, 0, 0, 1, 0, 1, 1, 1, 1);
	int *e = my_malloc(490);
	*e = 1;
	assert_freelist_count(8, 0, 0, 0, 0, 1, 1, 1, 1);
	my_free(a);
	assert_freelist_count(8, 0, 0, 0, 1, 0, 0, 0, 0);
	my_free(b);
	assert_freelist_count(8, 0, 0, 1, 1, 0, 0, 0, 0);
	my_free(c);
	assert_freelist_count(8, 0, 0, 2, 1, 0, 0, 0, 0);
	my_free(d);	
	printf("1\n");
	print_list(freelist);
	//assert_freelist_count(8, 0, 1, 1, 0, 0, 0, 0, 0);
	my_free(e);

	assert(ERRNO == NO_ERROR);
	//assert_freelist_count(8, 1, 0, 0, 0, 0, 0, 0, 0);

	fprintf(stderr, "%s\n", "--- PASSED MY_MALLOC, MY_FREE NORMAL CONDITIONS ---");

	// Test my_malloc SINGLE_REQUEST_TOO_LARGE
	assert(my_malloc(2049) == NULL);
	assert(ERRNO == SINGLE_REQUEST_TOO_LARGE);

	fprintf(stderr, "%s\n", "--- PASSED MY_MALLOC, SINGLE_REQUEST_TOO_LARGE ---");

	// Test my_malloc extending	heap size
	int *f = my_malloc(2000);
	*f = 5;
	int *g = my_malloc(2000);
	*g = 12;
	int *h = my_malloc(2000);
	*h = 13;
	int *i = my_malloc(2000);
	*i = 32;
	assert(ERRNO == NO_ERROR);

	// Test my_malloc OUT_OF_MEMORY
	assert(my_malloc(1990) == NULL);
	assert(ERRNO == OUT_OF_MEMORY);

	my_free(f);
	my_free(g);
	my_free(h);
	my_free(i);

	assert_freelist_count(8, 4, 0, 0, 0, 0, 0, 0, 0);

	fprintf(stderr, "%s\n", "--- PASSED MY_MALLOC, OUT_OF_MEMORY ---");

	// Test my_free, DOUBLE_FREE_DETECTED
	my_free(a);
	assert(ERRNO == DOUBLE_FREE_DETECTED);

	fprintf(stderr, "%s\n", "--- PASSED MY_FREE, DOUBLE_FREE_DETECTED ---");

	// Test my_memmove
	char* string1 = my_malloc(20);
	char* string2 = my_malloc(20);
	string1 = "test";
	my_memmove(string2, string1, 4);
	assert(strcmp(string2, string1) == 0);

	my_memmove((char *)string2 + 1, string1, 4);
	assert(strcmp(string2, "ttest") == 0);

	my_memmove((char *)string2 + 1, string2, 4);
	assert(strcmp(string2, "tttes") == 0);

	fprintf(stderr, "%s\n", "--- PASSED MY_MEMMOVE ---");

	// Test my_calloc
	char* calloc1 = my_calloc(2, 16);
	assert(zeroed_out(calloc1, 32) == 1);
	assert(ERRNO == NO_ERROR);

	char* calloc2 = my_calloc(20, 300);
	assert(calloc2 == NULL);
	assert(ERRNO == SINGLE_REQUEST_TOO_LARGE);

	fprintf(stderr, "%s\n", "--- PASSED MY_CALLOC ---");

	return 0;
}