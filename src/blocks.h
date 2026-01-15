#ifndef BLOCKS_H
#define BLOCKS_H

#include "game.h"

typedef struct block {
} block_t;

typedef struct blocks {
	block_t ***blocks;
	blocks_init_data_t init_data;
	size_t tex_id;
} blocks_t;

int blocks_init(
	blocks_t *blocks,
	blocks_init_data_t init_data,
	size_t tex_id
);

void blocks_del(blocks_t *blocks);

#endif
