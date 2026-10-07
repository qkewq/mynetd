#ifndef BITMASK_H
#define BITMASK_H

typedef struct bitmask_t{
	uint8_t *mask;
	size_t size;
} bitmask_t;

bitmask_t *bitmask_init(size_t num_bits);
int set_bit(bitmask_t *mask, size_t bit);
int unset_bit(bitmask_t *mask, size_t bit);
int is_set(bitmask_t *mask, size_t bit);
int bitmask_copy(bitmask_t *src, bitmask_t *dst);
void bitmask_clear(bitmask_t *mask);
void bitmask_free(bitmask_t *mask);

#endif
