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
    struct my_metadata_t *prev;
} my_metadata_t;

// Data of whole heap.
typedef struct my_heap_t
{
    my_metadata_t *free_head;
    my_metadata_t dummy;
} my_heap_t;

my_heap_t my_heap;

// Merges two free slots
my_metadata_t *merge_free_slots(my_metadata_t *metadata_left, my_metadata_t *metadata_right)
{
    assert(metadata_right == (char *)(metadata_left + 1) + metadata_left->size);
    metadata_left->size += (sizeof(my_metadata_t) + metadata_right->size);
    my_remove_from_free_list(metadata_right);
    return metadata_left;
}

// Adds a free slot to the beginning of the free list.
// |metadata|: pointer to the metadata of the free slot on the head.
void my_add_to_free_list(my_metadata_t *metadata)
{
    assert(!metadata->next); // Checks if |metadata| is not in the free list.
    metadata->next = my_heap.free_head;
    metadata->prev = NULL;
    my_heap.free_head->prev = metadata;
    my_heap.free_head = metadata;

    // Merges the added free slot with a neighboring one.
    my_metadata_t *metadata_to_merge = my_heap.free_head;
    while (metadata_to_merge)
    {
        if (!metadata_to_merge)
            break;
        if (metadata_to_merge == (char *)(metadata + 1) + metadata->size)
        {
            // Merges with a free slot on the right.
            my_metadata_t *next = metadata_to_merge->next;
            metadata = merge_free_slots(metadata, metadata_to_merge);
            metadata_to_merge = next;
        }
        else if (metadata == (char *)(metadata_to_merge + 1) + metadata_to_merge->size)
        {
            // Merges with a free slot on the left.
            metadata = merge_free_slots(metadata_to_merge, metadata);
            metadata_to_merge = metadata_to_merge->next;
        }
        else
        {
            metadata_to_merge = metadata_to_merge->next;
        }
    }
}

// Removes a free slot from the free list.
void my_remove_from_free_list(my_metadata_t *metadata)
{
    if (metadata->prev)
    {
        metadata->prev->next = metadata->next;
    }
    else
    { // When removing free slot is on the head.
        my_heap.free_head = metadata->next;
    }
    metadata->next->prev = metadata->prev;
    metadata->next = NULL;
    metadata->prev = NULL;
}

// my_initialize() is called only once at the beginning of each challenge.
void my_initialize()
{
    my_heap.free_head = &my_heap.dummy;
    my_heap.dummy.size = 0;
    my_heap.dummy.next = NULL;
    my_heap.dummy.prev = NULL;
}

// my_malloc() is called every time an object is allocated. |size| is guaranteed
// to be a multiple of 8 bytes and meets 8 <= |size| <= 4000. You are not
// allowed to use any library functions other than mmap_from_system /
// munmap_to_system.
void *my_malloc(size_t size)
{
    my_metadata_t *metadata = my_heap.free_head;

    // Best-fit: Find the smallest free slot the object fits.
    my_metadata_t *smallest_metadata = NULL;
    while (metadata)
    {
        metadata = metadata->next;
        if (!metadata)
            break;

        if (metadata->size >= size)
        {
            if (smallest_metadata == NULL)
            {
                smallest_metadata = metadata;
            }
            else if (metadata->size < smallest_metadata->size)
            {
                smallest_metadata = metadata;
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
        // Add the memory region to the free list.
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
    my_remove_from_free_list(smallest_metadata);

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

// my_free() is called every time an object is freed.  You are not allowed to
// use any library functions other than mmap_from_system / munmap_to_system.
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
