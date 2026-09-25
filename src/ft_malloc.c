#include "ft_malloc.h"

t_allocator g_allocator;

void *ft_malloc(size_t size) {

	if (size <= TINY_MAX)
		return allocate(size, &g_allocator.tiny, TINY_BLOCK_SIZE, TINY_BLOCK_COUNT);
	else if (size <= SMALL_MAX)
		return allocate(size, &g_allocator.small, SMALL_BLOCK_SIZE, SMALL_BLOCK_COUNT);
	else if (size > SMALL_MAX)
		return allocate_large(size, &g_allocator.large);
	return NULL;
}

int main() {
	int *m = (int *)ft_malloc(-1);
	printf("m addr: %p\n", m);
}
