#include "all.h"
#include "bitfield.h"



void print_table(cell_t* table){
    printf("[\n");
    //uint8_t testarr[] = {2,2,2,2,2,2,2,2};
    for(size_t i = 0; i < 256; i++){
        printf("{");
        printf("char: '%c', ",isprint((char)i)?(char)i:' ');
        printf("code: %ld, ",i);
        printf("bits: ");
        cell_t cell = table[i];
        for(size_t i = 0; i < cell.size; i++){
            //printf("%d",buff_get_bit(testarr/*cell.bits*/,i));
            printf("%d",buff_get_bit(cell.bits,i));
        }
        printf("}\n");
    }
    printf("]\n");
}

void construct_table_kernel(node_t* tree, cell_t* table, uint8_t* buff, size_t buffpos){//buff records the bit sequence
    if(tree->left == NULL){//leaf node
        buff_copy(buff,table[tree->code].bits,TEMPBUFF_SIZE);
        table[tree->code].size = buffpos;
    }else{
        buff_write_bit(buff,buffpos,0);
        construct_table_kernel(tree->left, table, buff, buffpos+1);
        buff_write_bit(buff,buffpos,1);
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


buffer_t* pack_table(cell_t* table, size_t bodysize){
    size_t max_size = 0;
    for(size_t i = 0; i < 256; i++){
        cell_t cell = table[i];
        max_size = cell.size > max_size ? cell.size : max_size;
    }
    //the first 2 bytes are for max cell size
    //initializing the field
    buffer_t* field = buffer_construct(256*sizeof(cell_t)+bodysize);
    
    buffer_append_bits(&field,&max_size,16);//copy 16 bit
    for(size_t i = 0; i < 256; i++){
        cell_t cell = table[i];
        if(cell.size == 0){
            buffer_append_bit(&field,0);
        }else{
            buffer_append_bit(&field,1);
            buffer_append_bits(&field,&cell.size,max_size);//copy the size
            buffer_append_bits(&field,cell.bits,cell.size);//copy the size
        }
    }
    return field;
}




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