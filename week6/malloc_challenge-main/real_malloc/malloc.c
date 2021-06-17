////////////////////////////////////////////////////////////////////////////////
/*                 (๑＞◡＜๑)  Malloc Challenge!!  (◍＞◡＜◍)                   */
////////////////////////////////////////////////////////////////////////////////

//
// Welcome to Malloc Challenge!! Your job is to invent a smart malloc algorithm.
//
// Rules:
//
// 1. Your job is to implement my_malloc(), my_free() and my_initialize().
//   *  my_initialize() is called only once at the beginning of each challenge.
//      You can initialize the memory allocator.
//   *  my_malloc(size) is called every time an object is allocated. In this
//      challenge, |size| is guaranteed to be a multiple of 8 bytes and meets
//      8 <= size <= 4000.
//   * my_free(ptr) is called every time an object is freed.
//   * Additionally, my_finalize() is called only once at the end of each
//   challenge,
//     so you can use this function for doing some clean ups if you want.
// 2. The only library functions you can use in my_malloc() and my_free() are
//    mmap_from_system() and munmap_to_system().
//   *  mmap_from_system(size) allocates |size| bytes from the system. |size|
//      needs to be a multiple of 4096 bytes. mmap_from_system(size) is a
//      system call and heavy. You are expected to minimize the call of
//      mmap_from_system(size) by reusing the returned
//      memory region as much as possible.
//   *  munmap_to_system(ptr, size) frees the memory region [ptr, ptr + size)
//      to the system. |ptr| and |size| need to be a multiple of 4096 bytes.
//      You are expected to free memory regions that are unused.
//   *  You are NOT allowed to use any other library functions at all, including
//      the default malloc() / free(), std:: libraries etc. This is because you
//      are implementing malloc itself -- if you use something that may use
//      malloc internally, it will result in an infinite recurion.
// 3. simple_malloc(), simple_free() and simple_initialize() in simple_malloc.c
//    are an example of straightforward implementation.
//    Your job is to invent a smarter malloc algorithm than the simple malloc.
// 4. There are five challenges (Challenge 1, 2, 3, 4 and 5). Each challenge
//    allocates and frees many objects with different patterns. Your malloc
//    is evaluated by two criteria.
//   *  [Speed] How faster your malloc finishes the challange compared to
//      the simple malloc.
//   *  [Memory utilization] How much your malloc is memory efficient.
//      This is defined as (S1 / S2), where S1 is the total size of objects
//      allocated at the end of the challange and S2 is the total size of
//      mmap_from_system()ed regions at the end of the challenge. You can
//      improve the memory utilization by decreasing memory fragmentation and
//      reclaiming unused memory regions to the system with munmap_to_system().
// 5. This program works on Linux and Mac but not on Windows. If you don't have
//    Linux or Mac, you can use Google Cloud Shell (See
//    https://docs.google.com/document/d/1TNu8OfoQmiQKy9i2jPeGk1DOOzSVfbt4RoP_wcXgQSs/edit#).
// 6. You need to specify an '-lm' option to compile this program.
//   *  gcc malloc_challenge.c -lm
//   *  clang malloc_challenge.c -lm
//
// Enjoy! :D
//

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *mmap_from_system(size_t size);
void munmap_to_system(void *ptr, size_t size);

// Metadata for each free slot.
typedef struct my_metadata_t
{
  size_t size;
  struct my_metadata_t *next;
} my_metadata_t;

// Data of whole heap.
typedef struct my_heap_t
{
  my_metadata_t *free_head;
  my_metadata_t dummy;
} my_heap_t;

my_heap_t my_heap;

// Adds a free slot to the beginning of the free list.
// |metadata|: pointer to the metadata of the free slot on the head.
void my_add_to_free_list(my_metadata_t *metadata)
{
  assert(!metadata->next); // Checks if |metadata| is not in the free list.
  metadata->next = my_heap.free_head;
  my_heap.free_head = metadata;
}

// Removes a free slot from the free list.
// |metadata|: metadata of the removing free slot.
// |prev|: metadata of the free slot former to the removing one.
// |prev| is NULL if |metadata| is on the head of free list.
void my_remove_from_free_list(my_metadata_t *metadata, my_metadata_t *prev)
{
  if (prev)
  {
    prev->next = metadata->next;
  }
  else
  { // When removing free slot is on the head.
    my_heap.free_head = metadata->next;
  }
  metadata->next = NULL;
}

// my_initialize() is called only once at the beginning of each challenge.
void my_initialize()
{
  my_heap.free_head = &my_heap.dummy;
  my_heap.dummy.size = 0;
  my_heap.dummy.next = NULL;
}

// my_malloc() is called every time an object is allocated. |size| is guaranteed
// to be a multiple of 8 bytes and meets 8 <= |size| <= 4000. You are not
// allowed to use any library functions other than mmap_from_system /
// munmap_to_system.
void *my_malloc(size_t size)
{
  my_metadata_t *metadata = my_heap.free_head;
  my_metadata_t *prev = NULL; // Needed in my_remove_from_free_list().

  // // First-fit: Find the first free slot the object fits.
  // while (metadata && metadata->size < size) {
  //   prev = metadata;
  //   metadata = metadata->next;
  // }

  // Best-fit: Find the smallest free slot the object fits.
  my_metadata_t *smallest_metadata = NULL;
  my_metadata_t *smallest_prev = NULL;
  while (metadata)
  {
    prev = metadata;
    metadata = metadata->next;
    if (!metadata) break;

    if (metadata->size >= size)
    {
      if (smallest_metadata == NULL)
      {
        smallest_metadata = metadata;
        smallest_prev = prev;
      }
      else if (metadata->size < smallest_metadata->size)  // Change '<' to '>' to switch to worst-fit.
      {
        smallest_metadata = metadata;
        smallest_prev = prev;
      }
    }
  }

  // If no free slot available.
  // Requests a new memory region from the system by calling mmap_from_system().
  if (!smallest_metadata)
  {
    size_t buffer_size = 4096;
    my_metadata_t *new_metadata = (my_metadata_t *)mmap_from_system(buffer_size);
    new_metadata->size = buffer_size - sizeof(my_metadata_t);
    new_metadata->next = NULL;
    // Adds the memory region to the free list.
    my_add_to_free_list(new_metadata);
    // Now, try my_malloc() again. This should succeed.
    return my_malloc(size);
  }

  // Allocates |size| from the free slot.
  // NOTE: |smallest_metadata| remains. Just removed from the free list.
  // |smallest_metadata| will be used to maintain the size of the allocated slot.
  void *ptr = smallest_metadata + 1; // Beginning of the allocated object.
  size_t remaining_size = smallest_metadata->size - size;
  smallest_metadata->size = size;

  // Removes |smallest_metadata| from the free list.
  my_remove_from_free_list(smallest_metadata, smallest_prev);

  // Creates a new metadata if free slot remains.
  if (remaining_size > sizeof(my_metadata_t))
  {
    my_metadata_t *new_metadata = (my_metadata_t *)((char *)ptr + size);
    new_metadata->size = remaining_size - sizeof(my_metadata_t);
    new_metadata->next = NULL;

    // Adds the remaining free slot to the free list.
    my_add_to_free_list(new_metadata);
  }

  // Returns the pointer to the allocated slot.
  return ptr;
}

// my_free() is called every time an object is freed.  
void my_free(void *ptr)
{
  // The metadata is placed just prior to the object.
  my_metadata_t *metadata = (my_metadata_t *)ptr - 1;
  // Adds the free slot to the free list.
  my_add_to_free_list(metadata);
}

void my_finalize()
{
  // Implement here!
}

void test()
{
  // Implement here!
  assert(1 == 1); /* 1 is 1. That's always true! (You can remove this.) */
}
