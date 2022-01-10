#include "all.h"
#include "table.h"

cell_t* unpack_table(buffer_t* field){
    buffer_print(field,8);
    size_t cell_size_size = 0;
    printf("position: %ld %d\n",field->offset_bytes,field->offset_bits);
    buffer_read_bits(field, (uint8_t*)&cell_size_size, 16);
    printf("cell size size is: %ld\n",cell_size_size);
    printf("position: %ld %d\n",field->offset_bytes,field->offset_bits);
    cell_t* table = calloc(256, sizeof(cell_t));
    for(int i = 0; i < 256; i++){
        uint8_t bit = buffer_read_bit(field);
        if(!bit)//bit non-zero
            continue;
        size_t cell_size = 0;
        buffer_read_bits(field, (uint8_t*)&cell_size, cell_size_size);
        buffer_read_bits(field, table[i].bits, cell_size);
        table[i].size = cell_size;
    }
    buffer_print(field,8);
    return table;
}


node_t* unpack_node(buffer_t* field){
    buffer_print(field,8);
    size_t cell_size_size = 0;
    printf("position: %ld %d\n",field->offset_bytes,field->offset_bits);
    buffer_read_bits(field, (uint8_t*)&cell_size_size, 16);
    printf("cell size size is: %ld\n",cell_size_size);
    printf("position: %ld %d\n",field->offset_bytes,field->offset_bits);
    
    node_t* root = calloc(1,sizeof(node_t));
    uint8_t* buff = malloc(TEMPBUFF_SIZE);
    for(int i = 0; i < 256; i++){
        uint8_t bit = buffer_read_bit(field);
        if(!bit)//bit non-zero
            continue;
        size_t cell_size = 0;
        buffer_read_bits(field, (uint8_t*)&cell_size, cell_size_size);
        buffer_read_bits(field, buff, cell_size);
        node_t* node = root;
        printf("i: %d\n",i);
        for(size_t j = 0; j < cell_size; j++){
            uint8_t bit = buff_get_bit(buff,j);
            node_t* subnode;
            if(bit == 0){
                if(node->left == NULL){
                    subnode = calloc(1,sizeof(node_t));
                    node->left = subnode;
                }else{
                    subnode = node->left;
                }
            }else{
                if(node->right == NULL){
                    subnode = calloc(1,sizeof(node_t));
                    node->right = subnode;
                }else{
                    subnode = node->right;
                }
            }
            node = subnode;
        }
        node->code = (uint8_t)i;
    }
    buffer_print(field,8);
    free(buff);
    return root;
}



int main(int argc, char** argv){
    if(argc != 3)
        fprintf(stderr,"Usage: ./a.out archive destination\n");
        
    char* fname = argv[1];
    char* dname = argv[2];
    fprintf(stderr,"Reading the file: %s\n",fname);
    
    FILE* file = fopen(fname, "rb");
    fseek(file, 0, SEEK_END);
    size_t fsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    uint8_t* buff = malloc(fsize);
    fread(buff, fsize, 1, file);
    fclose(file);
    buffer_t* field = buffer_from(buff, fsize, 0, 0);
    
    size_t size;
    buffer_read_bits(field, (uint8_t*)&size, sizeof(size_t)*8);
    field->offset_bytes = 8;
    field->offset_bits = 0;
    
    /*cell_t* table = unpack_table(field);
    print_table(table);
    buffer_print(field,8);*/
    
    node_t* root = unpack_node(field);
    node_print_json(root);
    
    //decoding the file
    printf("decoded size: %ld\n",size);
    uint8_t* buff2 = malloc(size);
    
    for(size_t i = 0; i < size; i++){
        node_t* tree = root;
        while(tree->left != NULL){
            tree = buffer_read_bit(field)?tree->right:tree->left;
        }
        buff2[i] = tree->code;
        //printf("%c",tree->code);
    }
    
    fprintf(stderr,"Writing to the file: %s\n",dname);
    FILE* dfile = fopen(dname,"wb");
    if (dfile){
        fwrite(buff2, size, 1, dfile);
        fprintf(stderr,"Complete\n");
    }
    else{
        fprintf(stderr,"Error writing to the file: %s\n",dname);
    }
    
    //freeing things
    buffer_destruct(field);
    node_destruct_recursive(root);
}




