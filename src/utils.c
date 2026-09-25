#include "ft_malloc.h"

size_t round_up(size_t size, size_t pagesize) {
	return ((size + pagesize - 1) / pagesize) * pagesize;
}
