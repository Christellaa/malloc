#include "ft_malloc.h"

static void print_block_json(t_zone *zone, size_t idx, int is_large) {
	uintptr_t addr = (uintptr_t)(zone->first_block + idx * zone->block_size);
	int used;
	if (is_large)
		used = 1;
	else
		used = !is_block_free(zone, idx);
	int aligned = (addr % 16) == 0;

	printf("        {\n");
	printf("            \"index\": %zu,\n", idx);
	printf("            \"address\": \"0x%lx\",\n",
		(unsigned long)addr);
	printf("            \"used\": %s,\n",
		used ? "true" : "false");
	printf("            \"aligned\": %s\n",
		aligned ? "true" : "false");
	printf("        }");
}

static void print_zone_json(t_zone *zone, int is_large) {
	printf("    {\n");
	printf("        \"address\": \"0x%lx\",\n",
		(unsigned long)(uintptr_t)zone);
	printf("        \"zoneSize\": %zu,\n", zone->zone_size);
	printf("        \"blockSize\": %zu,\n", zone->block_size);
	printf("        \"blockCount\": %zu,\n", zone->block_count);

	printf("        \"blocks\": [\n");

	size_t i = 0;
	while (i < zone->block_count) {
		print_block_json(zone, i, is_large);
		if (i + 1 < zone->block_count)
			printf(",");
		printf("\n");
		i++;
	}
	printf("        ]\n");
	printf("    }");
}

void dump_zone_list(t_zone *zone, int is_large) {
	int first = 1;
	while (zone != NULL)
	{
		if (!first)
			printf(",\n");
		print_zone_json(zone, is_large);
		first = 0;
		zone = zone->next;
	}
}

void dump_allocator_json() {
	printf("{\n");
	printf("    \"alignment\": 16,\n");

	printf("    \"tiny\": [\n");
	dump_zone_list(g_allocator.tiny, 0);
	printf("\n    ],\n");

	printf("    \"small\": [\n");
	dump_zone_list(g_allocator.small, 0);
	printf("\n    ],\n");

	printf("    \"large\": [\n");
	dump_zone_list(g_allocator.large, 1);
	printf("\n    ]\n");

	printf("}\n");
}