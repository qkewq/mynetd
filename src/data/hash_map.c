#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "data/hash_map.h"

/*
Simple hash map
Only works with null terminated keys
Type agnostic values (up to the caller)
Minimal error checking
*/

uint32_t get_hash(char *key){ // good enough
	if(!key[0]){
		return 0;
	}

	int i = 0;
	uint32_t hash = 1024;
	while(key[i]){
		hash += (key[i] << 3);
		hash *= key[i];
		i++;
	}

	return hash;
}

hash_map_t *hash_map_init(size_t num_buckets){
	hash_map_t *new_map = calloc(1, sizeof(hash_map_t));
	if(!new_map){
		return NULL;
	}

	new_map->buckets = calloc(num_buckets, sizeof(hash_map_node_t));
	if(!new_map->buckets){
		free(new_map);
		return NULL;
	}

	new_map->num_buckets = num_buckets;
	return new_map;
}

int hash_map_insert(hash_map_t *map, char *key, void *value, size_t value_size){
	int key_size = strlen(key);

	hash_map_node_t *new_node = calloc(1, sizeof(hash_map_node_t));
	if(!new_node){
		return -1;
	}

	new_node->key = calloc(key_size + 1, sizeof(char));
	new_node->value = calloc(value_size, 1);

	if(!new_node->key || !new_node->value){
		free(new_node->key);
		free(new_node->value);
		free(new_node);
		return -1;
	}

	memcpy(new_node->key, key, key_size);
	memcpy(new_node->value, value, value_size);
	new_node->value_size = value_size;

	uint32_t index = get_hash(key) % map->num_buckets;
	new_node->next = map->buckets[index];
	map->buckets[index] = new_node;

	return 0;
}

void *hash_map_lookup(hash_map_t *map, char *key, size_t *return_size){
	uint32_t index = get_hash(key) % map->num_buckets;

	hash_map_node_t *current = map->buckets[index];
	hash_map_node_t *next = NULL;
	char *value = NULL;
	size_t value_size = 0;

	while(current){
		next = current->next;
		if(strcmp(key, current->key) == 0){
			value = current->value;
			value_size = current->value_size;
			break;
		}
		current = next;
	}

	*return_size = value_size;
	return value;
}

void hash_map_free(hash_map_t *map){
	hash_map_node_t *current = NULL;
	hash_map_node_t *next = NULL;
	for(int i = 0; i < map->num_buckets; i++){
		current = map->buckets[i];
		while(current){
			next = current->next;
			free(current->key);
			free(current->value);
			free(current);

			current = next;
		}
	}
	free(map->buckets);
	free(map);
}
