#ifndef FT_MALLOC_H
#define FT_MALLOC_H

#include <stdio.h> // printf // TODO: to delete
#include <sys/mman.h> // mmap, munmap
#include <unistd.h> // _SC_PAGE_SIZE, sysconf
#include <stdint.h> // SIZE_MAX

#define TINY_MAX 128
#define TINY_BLOCK_SIZE 128
#define TINY_BLOCK_COUNT 100

#define SMALL_MAX 2096
#define SMALL_BLOCK_SIZE 232
#define SMALL_BLOCK_COUNT 100

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

t_allocator	g_allocator;

#endif