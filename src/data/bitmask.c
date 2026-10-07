#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "data/bitmask.h"

bitmask_t *bitmask_init(size_t num_bits){
	size_t num_bytes = (num_bits + 7) / 8;

	bitmask_t *mask = calloc(1, sizeof(bitmask_t));
	if(!mask){
		return NULL;
	}

	mask->mask = calloc(num_bytes, 1);
	if(!mask->mask){
		free(mask);
		return NULL;
	}

	mask->size = num_bytes;
	return mask;
}

int validate_size(size_t size, size_t bit){
	if(bit / 8 > size - 1){
		return 1;
	}

	return 0;
}

int set_bit(bitmask_t *mask, size_t bit){
	if(!validate_size(mask->size, bit)){
		return 0;
	}

	mask->mask[bit / 8] |= (1 << (bit % 8));
	return 1;
}

int unset_bit(bitmask_t *mask, size_t bit){
	if(!validate_size(mask->size, bit)){
		return 0;
	}

	mask->mask[bit / 8] &= ~(1 << (bit % 8));
	return 1;
}

int is_set(bitmask_t *mask, size_t bit){
	if(!validate_size(mask->size, bit)){
		return 0;
	}

	if(mask->mask[bit / 8] & (1 << (bit % 8))){
		return 1;
	}

	return 0;
}

int bitmask_copy(bitmask_t *src, bitmask_t *dst){
	if(src->size != dst->size){
		return 0;
	}

	memcpy(dst->mask, src->mask, dst->size);
	return 1;
}

void bitmask_clear(bitmask_t *mask){
	memset(mask->mask, 0, mask->size);
}

void bitmask_free(bitmask_t *mask){
	free(mask->mask);
	free(mask);
}
