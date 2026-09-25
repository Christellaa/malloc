#include "ft_malloc.h"

int is_block_free(t_zone *zone, size_t i) {
	size_t byte = i / 8;
	size_t bit = i % 8;

	/**
	 * eg:
	 * i = 10
	 * i / 8 bits = 10 / 8 = 1
	 * zone->bitmap[1] = block 10 is in byte 1 of the bitmap
	 * 
	 * i % 8 bits = 10 % 8 = 2
	 * 1 << 2 = create a mask with only bit 2 set = 00000100
	 * zone->bitmap[1] & 00000100 = is bit 2 of byte 1 set?
	 * eg real bitmap: 10111010, mask: 00000100, after AND operation: 00000000 = not set
	 */
	return (zone->bitmap[byte] & (1 << (bit))) == 0;
}

size_t find_free_block(t_zone *zone) {
	for (size_t i = 0; i < zone->block_count; i++) {
		if (is_block_free(zone, i))
			return i;
	}
	return zone->block_count;
}

void mark_block_as_used(t_zone *zone, size_t idx) {
	size_t byte = idx / 8;
	size_t bit = idx % 8;
	/**
	 * eg:
	 * i = 10
	 * i / 8 bits = 10 / 8 = 1
	 * zone->bitmap[1] = block 10 is in byte 1 of the bitmap
	 * 
	 * i % 8 bits = 10 % 8 = 2
	 * 1 << 2 = create a mask with only bit 2 set = 00000100
	 * zone->bitmap[1] |= 00000100 = take the existing byte and make sure bit 2 is 1 (= set as used)
	 * eg real bitmap: 10111010, mask: 00000100, after OR operation: 10111110 = set as used
	 */
	zone->bitmap[byte] |= (1 << (bit));
}

void *allocate(
	size_t size,
	t_zone **zones,
	size_t block_size,
	size_t block_count
) {
	t_zone *zone;
	size_t idx;
	(void)size;

	if (*zones == NULL)
	{
		zone = create_zone(block_size, block_count);
		if (zone == NULL)
			return NULL;

		append_zone(zones, zone);
	}

	zone = *zones;
	while (zone != NULL) {
		idx = find_free_block(zone);
		if (idx < zone->block_count) {
			mark_block_as_used(zone, idx);
			return zone->first_block + idx * zone->block_size;
		}
		zone = zone->next;
	}

	zone = create_zone(block_size, block_count);
	if (zone == NULL)
		return NULL;

	append_zone(zones, zone);
	mark_block_as_used(zone, 0);
	return zone->first_block;
}

void *allocate_large(size_t size, t_zone **zones) {
	size_t pagesize = sysconf(_SC_PAGE_SIZE);

	if (size > (SIZE_MAX - sizeof(t_zone)))
		return NULL;

	size_t zone_size = round_up(sizeof(t_zone) + size, pagesize);
	if (zone_size == 0)
		return NULL;

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
	zone->block_size = size;
	zone->block_count = 1;
	zone->bitmap_size = 0;
	zone->bitmap = NULL;
	zone->first_block = (char *)zone + sizeof(t_zone);
	zone->next = NULL;

	append_zone(zones, zone);

	return zone->first_block;
}
