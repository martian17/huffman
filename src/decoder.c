#include "all.h"
#include "table.h"

cell_t* inflate_table(buffer_t* field){
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
    
    cell_t* table = inflate_table(field);
    print_table(table);
    
    
    
    
    //deflate the huffman table
    
    
    /*if(argc != 3)
        fprintf(stderr,"Usage: ./a.out destname filename\n");
        
    char* dname = argv[1];
    char* fname = argv[2];
    fprintf(stderr,"Reading the file: %s\n",fname);
    
    
    FILE* file = fopen(fname, "rb");
    fseek(file, 0, SEEK_END);
    size_t fsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    uint8_t* buff = malloc(fsize);
    fread(buff, fsize, 1, file);
    fclose(file);
    
    //printing the incoming buffer
    for(size_t i = 0; i < fsize; i++){
        printf("%c",buff[i]);
    }
    
    node_t* tree = construct_huffman_tree(buff,fsize);
    
    printNodeJSON(tree);
    printf("\n");
    
    cell_t* table = construct_table(tree);
    print_table(table);
    
    //uint8_t* result = malloc(256*sizeof(cell_t));
    buffer_t* result = pack_table(table, fsize);
    buffer_print(result,8);
    for(size_t i = 0; i < fsize; i++){
        uint8_t byte = buff[i];
        buffer_append_bits(result,table[byte].bits,table[byte].size);
    }
    buffer_print(result,8);
    
    
    fprintf(stderr,"Writing to the file: %s\n",dname);
    FILE* dfile = fopen(dname,"wb");
    if (dfile){
        size_t size = result->offset_bytes+(result->offset_bits==0?0:1);
        fwrite(result->buff, size, 1, dfile);
        fprintf(stderr,"Complete");
    }
    else{
        fprintf(stderr,"Error writing to the file: %s\n",dname);
    }
    
    
    //write a compressed table as the header
    //for(){
    //    
    //}
    
    free_node_recursive(tree);*/
    printf("Hello friend\n");
}




