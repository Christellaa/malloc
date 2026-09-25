#include "ft_malloc.h"

size_t round_up(size_t size, size_t pagesize) {
	return ((size + pagesize - 1) / pagesize) * pagesize;
}

size_t align_block(size_t size, size_t alignment) {
	return (size + alignment - 1) & ~(alignment - 1);
}
