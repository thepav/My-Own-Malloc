My Own Malloc
===============

This is a C implementation of a Malloc on a unix system. This malloc implementation is done by use of a freelist and the Buddy System algorithm of memory allocation and deallocation. All major functionality of memory management is implemented including the following:

- Malloc
- Free
- Memmove
- Calloc

The following is an explaination of the freelist implementation explained by the Computer Systems and Programming at the Georgia Institute of Technology. 

The Freelist
============
When we allocate memory or take pieces of blocks we already allocated, there may be blocks we don't automatically use. For this reason, we keep a structure called the freelist that holds metadata about blocks that aren't currently in use.
The freelist is an array with eight elements - eight lists of blocks of distinct sizes. For the purposes of this assignment, the smallest block should be of size 16 bytes. 

This is what your freelist array should look like:
---------------------------------------------
|pointer to free list of blocks of size 16  |
---------------------------------------------
---------------------------------------------
|pointer to free list of blocks of size 32  |
---------------------------------------------
---------------------------------------------
|pointer to free list of blocks of size 64  |
---------------------------------------------
---------------------------------------------
|pointer to free list of blocks of size 128 |
---------------------------------------------
...
---------------------------------------------
|pointer to free list of blocks of size 2048|
---------------------------------------------

These pointers just exist to maintain links between blocks that are free. It is okay if a high address comes before a low address in the freelist for any block size, so long as the freelist holds all the blocks that are free at any time.


Install
=====
The installation of this code is simple. There is no extra setup needed since none of the function names in this code use the same names as system commands. Simply add 'my_' to the beginning of a command implemented in this code. ex. my_malloc instead of malloc. Make sure to include the 
