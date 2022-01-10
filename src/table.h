#ifndef TABLE_H
#define TABLE_H

#include "buffer.h"


#define TEMPBUFF_SIZE 32

typedef struct {
    uint8_t bits[TEMPBUFF_SIZE];
    size_t size;
} cell_t;


void print_table(cell_t* table);
//buff records the bit sequence
void construct_table_kernel(node_t* tree, cell_t* table, uint8_t* buff, size_t buffpos);
cell_t* construct_table(node_t* tree);
buffer_t* pack_table(cell_t* table, size_t bodysize);

#endif