#ifndef HASH_MAP_H
#define HASH_MAP_H

typedef struct hash_map_node_t{
	struct hash_map_node_t *next;
	char *key;
	size_t value_size;
	void *value;
} hash_map_node_t;

typedef struct hash_map_t{
	size_t num_buckets;
	hash_map_node_t **buckets;
} hash_map_t;

hash_map_t *hash_map_init(size_t num_buckets);
int hash_map_insert(hash_map_t *map, char *key, void *value, size_t value_size);
void *hash_map_lookup(hash_map_t *map, char *key, size_t *return_size);
void hash_map_free(hash_map_t *map);

#endif
