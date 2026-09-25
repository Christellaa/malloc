#ifndef FT_MALLOC_H
#define FT_MALLOC_H

/*** INCLUDES ***/

#include <stdio.h> // printf // TODO: to delete
#include <sys/mman.h> // mmap, munmap
#include <unistd.h> // _SC_PAGE_SIZE, sysconf
#include <stdint.h> // SIZE_MAX

/*** DEFINES ***/

#define TINY_MAX 128
#define TINY_BLOCK_SIZE 128
#define TINY_BLOCK_COUNT 100

#define SMALL_MAX 2096
#define SMALL_BLOCK_SIZE 232
#define SMALL_BLOCK_COUNT 100

/*** STRUCTURES ***/

typedef struct	s_zone {
	size_t			zone_size;
	size_t			block_size;
	size_t			block_count;
	size_t			bitmap_size;
	unsigned char	*bitmap;
	char			*first_block;
	struct s_zone	*next;
} t_zone;

typedef struct	s_allocator {
	t_zone	*tiny;
	t_zone	*small;
	t_zone	*large;
} t_allocator;

extern t_allocator	g_allocator;

/*** FUNCTIONS ***/

// allocate
void	*allocate(
	size_t size,
	t_zone **zones,
	size_t block_size,
	size_t block_count
);
void *allocate_large(size_t size, t_zone **zones);
// zones
void append_zone(t_zone **list, t_zone *new_zone);
void *create_zone(size_t block_size, size_t block_count);
// utils
size_t round_up(size_t size, size_t pagesize);

#endif