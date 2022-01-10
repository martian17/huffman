#include "all.h"
#include "table.h"
#include "mathutil.h"



void print_table(cell_t* table){
    printf("[\n");
    for(size_t i = 0; i < 256; i++){
        printf("{");
        printf("char: '%c', ",isprint((char)i)?(char)i:' ');
        printf("code: %ld, ",i);
        printf("bits: ");
        cell_t cell = table[i];
        buff_print(cell.bits,cell.size);
        printf("}\n");
    }
    printf("]\n");
}

void construct_table_kernel(node_t* tree, cell_t* table, uint8_t* buff, size_t buffpos){//buff records the bit sequence
    if(tree->left == NULL){//leaf node
        buff_copy(buff,table[tree->code].bits,TEMPBUFF_SIZE);
        table[tree->code].size = buffpos;
    }else{
        buff_set_bit(buff,buffpos,0);
        construct_table_kernel(tree->left, table, buff, buffpos+1);
        buff_set_bit(buff,buffpos,1);
        construct_table_kernel(tree->right, table, buff, buffpos+1);
    }
}

cell_t* construct_table(node_t* tree){
    cell_t* table = malloc(256*sizeof(cell_t));
    uint8_t* buff = malloc(TEMPBUFF_SIZE);
    construct_table_kernel(tree,table,buff,0);
    free(buff);
    return table;
}


void pack_table(buffer_t* field, cell_t* table){
    size_t max_size = 0;
    for(size_t i = 0; i < 256; i++){
        cell_t cell = table[i];
        max_size = cell.size > max_size ? cell.size : max_size;
    }
    
    size_t cell_size_size = log2ceil(max_size);
    buffer_append_bits(field,(uint8_t*)&cell_size_size,16);//copy 16 bit
    
    for(size_t i = 0; i < 256; i++){
        cell_t cell = table[i];
        if(cell.size == 0){
            buffer_append_bit(field,0);
        }else{
            buffer_append_bit(field,1);
            buffer_append_bits(field,(uint8_t*)&cell.size,cell_size_size);//copy the size
            buffer_append_bits(field,cell.bits,cell.size);//copy the size
        }
    }
}