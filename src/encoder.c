#include "all.h"
#include "table.h"



//minheap methods
node_t** minheap_construct(size_t length){
    node_t** heap = calloc(length+1, sizeof(size_t));
    heap[0] = malloc(sizeof(node_t));
    heap[0]->cnt = SIZE_MAX;
    return heap;
}

void minheap_destruct(node_t** heap){
    free(heap[0]);
    free(heap);
}


void minheap_push(node_t** heap, size_t* tail0, node_t* node){
    (*tail0)++;
    size_t tail = (*tail0);
    node_t* parent = heap[tail/2];
    while(parent->cnt > node->cnt && parent->cnt != SIZE_MAX){
        heap[tail] = parent;
        tail = tail/2;
        parent = heap[tail/2];
    }
    heap[tail] = node;
}

node_t* minheap_pop(node_t** heap, size_t* tail0){
    size_t tail = *tail0;
    if(tail < 2){
        if(tail == 1){
            node_t* result = heap[1];
            (*tail0)--;
            return result;
        }
        fprintf(stderr,"out of things to pop\n");
        exit(1);
    }
    (*tail0)--;
    node_t* result = heap[1];
    size_t head = 1;
    node_t* tail_node = heap[tail];
    tail--;
    while(head*2 < tail){
        size_t left = head*2;
        size_t right = head*2+1;
        node_t* left_node = heap[left];
        node_t* right_node = heap[right];
        size_t next;
        node_t* next_node;
        if(left_node->cnt < right_node->cnt){
            next_node = left_node;
            next = left;
        }else{
            next_node = right_node;
            next = right;
        }
        if(next_node->cnt < tail_node->cnt){
            heap[head] = next_node;
            head = next;
        }else{
            heap[head] = tail_node;
            return result;
        }
    }
    if(head*2 == tail && heap[head*2]->cnt < tail_node->cnt){
        heap[head] = heap[tail];
        heap[tail] = tail_node;
    }else{
        heap[head] = tail_node;
    }
    return result;
}

//tally the bytes
node_t** tally(uint8_t* str, size_t size){//returns a ll of nodes
    size_t* table = calloc(256, sizeof(size_t));
    for(size_t i = 0; i < size; i++){
        table[str[i]]++;
    }
    //construct the tree
    for(size_t i = 0; i < 256; i++){
        size_t cnt = table[i];
        node_t* node = malloc(sizeof(node_t));
        table[i] = (size_t)node;
        node->code = i;
        node->cnt = cnt;
        node->left = NULL;
        node->right = NULL;
    }
    return (node_t**)table;
}


node_t* construct_huffman_tree(uint8_t* buff, size_t size){
    node_t** nodes = tally(buff,size);
    
    //printint nodes
    for(size_t i = 0; i < 256; i++){
        node_fprint(nodes[i]);
        fprintf(stderr,"\n");
    }
    fprintf(stderr,"\n");
    
    //convert this to a binary heap
    node_t** heap = minheap_construct(256);
    size_t tail = 0;
    
    for(size_t i = 0; i < 256; i++){
        minheap_push(heap,&tail,nodes[i]);
    }
    
    //nodes no longer needed
    free(nodes);
    
    //delete the nodes with 0 cnt value
    for(size_t i = 0; i < 256; i++){
        if(heap[1]->cnt != 0){
            break;
        }
        node_t* node = minheap_pop(heap,&tail);
        free(node);
    }
    
    //printing the current length
    fprintf(stderr,"tail: %zu\n",tail);
    //printint nodes
    for(size_t i = 0; i <= tail; i++){
        node_fprint(heap[i]);
        fprintf(stderr,"\n");
    }
    fprintf(stderr,"\n");
    
    
    //constructing a tree
    while(tail != 1){//until there is a single element left in the heap
        node_t* node1 = minheap_pop(heap,&tail);
        node_t* node2 = minheap_pop(heap,&tail);
        node_t* node = malloc(sizeof(node_t));
        node->left = node1;
        node->right = node2;
        node->cnt = node1->cnt+node2->cnt;
        minheap_push(heap,&tail,node);
    }
    node_t* tree = heap[1];
    //free the heap
    minheap_destruct(heap);
    //now we have a clean tree to work with
    return tree;
}




int main(int argc, char** argv){
    if(argc != 3)
        fprintf(stderr,"Usage: ./a.out destname filename\n");
        
    char* dname = argv[1];
    char* fname = argv[2];
    fprintf(stderr,"Reading the file: %s\n",fname);
    
    
    FILE* file = fopen(fname, "rb");
    fseek(file, 0, SEEK_END);
    size_t fsize = ftell(file);
    fseek(file, 0, SEEK_SET);  /* same as rewind(f); */

    uint8_t* buff = malloc(fsize);
    fread(buff, fsize, 1, file);
    fclose(file);
    
    node_t* tree = construct_huffman_tree(buff,fsize);
    
    node_print_json(tree);
    printf("\n");
    
    cell_t* table = construct_table(tree);
    print_table(table);
    
    //the first 2 bytes are for max cell size
    //initializing the field
    buffer_t* field = buffer_construct(8+256*sizeof(cell_t)+fsize);
    //reserving space for the size
    field->offset_bytes = 8;
    pack_table(field, table);
    buffer_print(field,8);
    for(size_t i = 0; i < fsize; i++){
        uint8_t byte = buff[i];
        buffer_append_bits(field,table[byte].bits,table[byte].size);
    }
    size_t size = field->offset_bytes+(field->offset_bits==0?0:1);
    field->offset_bytes = 0;
    field->offset_bits = 0;
    buffer_append_bits(field,(uint8_t*)&fsize,sizeof(size_t)*8);
    
    fprintf(stderr,"Writing to the file: %s\n",dname);
    FILE* dfile = fopen(dname,"wb");
    if (dfile){
        fwrite(field->buff, size, 1, dfile);
        fprintf(stderr,"Complete\n");
    }
    else{
        fprintf(stderr,"Error writing to the file: %s\n",dname);
    }
    
    //freeing things
    buffer_destruct(field);
    node_destruct_recursive(tree);
}




