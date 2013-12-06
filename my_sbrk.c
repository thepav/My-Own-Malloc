#include <errno.h>
#include <stdlib.h>

/* emulates a call to the system call sbrk(2) */

/* we emulate for ease of debugging - if the allocator used the system
 * call sbrk then we would need to redefine the system malloc call to 
 * be ours instead to avoid conflicts. if our library did not work correctly,
 * then printf could not even perform simple tasks since it internally
 * dynamically allocates memory when formatting strings!! :-)
 */

/* 0x2000 base 16 = 8192 base 10 = 8 KB */
#define HEAP_SIZE 0x2000

void *my_sbrk(int increment) {

  static char *fake_heap = NULL;
  static int current_top_of_heap = 0;
  void *ret_val;

  if(fake_heap == NULL){
    if((fake_heap = calloc(HEAP_SIZE, 1)) == NULL) {
      return (void*)-1;
    }
  }
  ret_val=current_top_of_heap+fake_heap;
  if ((current_top_of_heap + increment > HEAP_SIZE) 
      || (current_top_of_heap+increment < 0)) {
    errno=ENOMEM;
    return (void*)-1;
  }
  current_top_of_heap += increment;
  return ret_val;
}
