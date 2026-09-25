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
	// tiny
	void *t_a = ft_malloc(42);
	void *t_b = ft_malloc(64);
	void *t_c = ft_malloc(128);
	void *t_d = ft_malloc(128);
	void *t_e = ft_malloc(128);
	void *t_f = ft_malloc(128);
	for (int i = 0; i < 100; i++)
		ft_malloc(128);
	// small
	void *s_a = ft_malloc(500);
	// large
	void *l_a = ft_malloc(5500);
	void *l_b = ft_malloc(5700);
	void *l_c = ft_malloc(8700);
	void *l_d = ft_malloc(1400);

	(void)t_a;
	(void)t_b;
	(void)t_c;
	(void)t_d;
	(void)t_e;
	(void)t_f;

	(void)s_a;

	(void)l_a;
	(void)l_b;
	(void)l_c;
	(void)l_d;

	dump_allocator_json();
}
