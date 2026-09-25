#include "ft_malloc.h"

void append_zone(t_zone **list, t_zone *new_zone) {
	if (*list == NULL)
	{
		*list = new_zone;
		return ;
	}

	t_zone *current = *list;
	while (current->next != NULL)
		current = current->next;
	current->next = new_zone;
}

void *create_zone(size_t block_size, size_t block_count) {
	size_t pagesize = sysconf(_SC_PAGE_SIZE);

	// get bitmap size
	// eg: 100 blocks -> 13 bytes
	size_t bitmap_size = (block_count + 7) / 8;
	// get raw size
	// metadata + bitmap + blocks
	size_t raw_size = sizeof(t_zone)
		+ bitmap_size
		+ block_size * block_count;
	// get zone size
	// mmap size is a multiple of page size
	size_t zone_size = round_up(raw_size, pagesize);

	t_zone *zone = mmap(
		NULL,
		zone_size,
		PROT_READ | PROT_WRITE,
		MAP_ANONYMOUS | MAP_PRIVATE,
		-1,
		0
	);
	if (zone == MAP_FAILED)
		return NULL;

	zone->zone_size = zone_size;
	zone->block_size = block_size;
	zone->block_count = block_count;
	zone->bitmap_size = bitmap_size;
	zone->bitmap = (unsigned char *)(zone + 1);
	for (size_t i = 0; i < bitmap_size; i++)
		zone->bitmap[i] = 0;
	zone->first_block = (char *)zone + sizeof(t_zone) + zone->bitmap_size;
	zone->next = NULL;

	return zone;
}
