#include "all.h"
#include "table.h"
#include "mathutil.h"



void print_table(cell_t* table){
    printf("[\n");
    //uint8_t testarr[] = {2,2,2,2,2,2,2,2};
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
    //printf("size before: %ld %d\n", field->offset_bytes, field->offset_bits);
    buffer_append_bits(field,(uint8_t*)&cell_size_size,16);//copy 16 bit
    
    //printf("size after: %ld %d\n", field->offset_bytes, field->offset_bits);
    for(size_t i = 0; i < 256; i++){
        cell_t cell = table[i];
        if(cell.size == 0){
            buffer_append_bit(field,0);
        }else{
            if(i == 46){
                buff_print(cell.bits,cell.size);
                printf("\n");
                buffer_print(field,8);
            }
            buffer_append_bit(field,1);
            if(i == 46){
                buffer_print(field,8);
            }
            buffer_append_bits(field,(uint8_t*)&cell.size,cell_size_size);//copy the size
            if(i == 46){
                buffer_print(field,8);
            }
            buffer_append_bits(field,cell.bits,cell.size);//copy the size
        }
    }
}



//arcane record that tried to use bitfield.c for this
/*
void construct_table_kernel(node_t* tree, bitfield_t* table, bitfield_t* cnt, size_t buffpos, size_t buffsize){//buff records the bit sequence
    if(tree->left == NULL){//leaf node
        copy_buff(buff,table[tree->code].bits,buffsize);
        table[tree->code].size = buffpos;
    }else{
        write_bit(buff,buffpos,0);
        construct_table_kernel(tree->left, table, buff, buffpos+1, buffsize);
        write_bit(buff,buffpos,1);
        construct_table_kernel(tree->right, table, buff, buffpos+1, buffsize);
    }
}


bitfield_t* construct_table(node_t* tree){
    bitfield_t* table = malloc(256*sizeof(bitfield_t));
    bitfield_t* cnt = bitfield_construct(32);//32 bit bitfield
    construct_table_kernel(tree,table,cnt
    );
    bitfield_destruct(cnt);
    return table;
}*/