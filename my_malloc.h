#ifndef __MY_MALLOC_H__
#define __MY_MALLOC_H__

/* we need this *ONLY* for the size_t typedef */
#include <stdio.h>

/* our metadata structure for use with the buddy system.
 * you *MUST NOT* change this definition unless specified
 * in an official assignment update by the TAs on the newsgroup
 * '.announce'!
 */
typedef struct metadata
{
  short in_use;
  short size;
  struct metadata* next;
  struct metadata* prev;
} metadata_t;

/* This is your error enum. You must return one of these in the
 * case that a call to your functions results in an error. The four
 * different types of errors for this homework are explained below.
 * If ANY function has a case where one of the erorrs described could
 * occur, it must return the appropriate enum CASTED AS THE PROPER 
 * RETURN TYPE. In the case where a single request is too large and
 * the request results in an out of memory error as well, the 
 * SINGLE_REQUEST_TOO_LARGE should take precedence. If any of the four
 * functions complete successfully, the error code should be set to
 * NO_ERROR. */
enum my_malloc_err {
	NO_ERROR,
	OUT_OF_MEMORY,
	SINGLE_REQUEST_TOO_LARGE,
	DOUBLE_FREE_DETECTED
};
enum my_malloc_err ERRNO;

/* this function should allocate a block that is big enough to hold the
 * specified size, and that is all. if there is not a block that is able
 * to satisfy the request, then you should attempt to grab more heap 
 * space with a call to my_sbrk. if this succeeds, then you should continue
 * as normal. if it fails, then you should return NULL.
 */
void* my_malloc(size_t);

/* this function should free the block of memory, recursively merging
 * buddies up the freelist until they can be merged no more.
 */
void my_free(void *);

/* this function returns a pointer to an array of num quantity of elements 
 * each of size size*/
void* my_calloc( size_t num, size_t size);

/* this copies memory starting at the source, src and copying num_bytes
 * of bytes into the memory pointed to by dest. The source and destination
 * can indeed overlap, so the copy should be done as if the data were put into
 * a temporary buffer first and then copied. */
void* my_memmove(void* dest, const void* src, size_t num_bytes);

/* this function will emulate the system call sbrk(2). if you do not
 * have enough free heap space to satisfy a memory request, then you
 * must call this function to allocate memory to your allocator. sound
 * odd? hopefully reading the NFO file will help you understand this.
 */
void* my_sbrk(int);
void print_list(metadata_t* list[8]);

static inline double fastlog2 (double x)
{
    union { float f; int i; } vx = { x };
    union { int i; float f; } mx = { (vx.i & 0x007FFFFF) | 0x3f000000 };
    float y = vx.i;
    y *= 1.1920928955078125e-7f;

    return y - 124.22551499f
             - 1.498030302f * mx.f 
             - 1.72587999f / (0.3520887068f + mx.f);
}
extern metadata_t* freelist[8];

#endif /* __MY_MALLOC_H__ */
