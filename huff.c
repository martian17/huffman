#include "all.h"
#include "buffer.h"
#include "table.h"


typedef struct{
    char code;
    size_t cnt;
    void* left;
    void* right;
} node_t;

void free_node_recursive(node_t* node){
    if(node->left != NULL){
        free_node_recursive(node->left);
    }
    if(node->right != NULL){
        free_node_recursive(node->right);
    }
    free(node);
}

void printNode(node_t* node){
    if(node == NULL){
        printf("0");
        return;
    }
    printf("{");
    if(node->left == NULL && node->right == NULL){//leaf node
        printf("char:'%c',",isprint(node->code)?node->code:' ');
        printf("cnt:%zu,code:%d",node->cnt,(uint8_t)node->code);
    }else{//stem node
        printf("cnt:%zu",node->cnt);
        printf(",left:");
        printNode(node->left);
        printf(",right:");
        printNode(node->right);
    }
    printf("}");
}

void fprintNode(node_t* node){
    if(node == NULL){
        printf("0");
        return;
    }
    fprintf(stderr,"{");
    if(node->left == NULL && node->right == NULL){//leaf node
        fprintf(stderr,"char:'%c',",isprint(node->code)?node->code:' ');
        fprintf(stderr,"cnt:%zu,code:%d",node->cnt,(uint8_t)node->code);
    }else{//stem node
        fprintf(stderr,"cnt:%zu",node->cnt);
        fprintf(stderr,",left:");
        fprintNode(node->left);
        fprintf(stderr,",right:");
        fprintNode(node->right);
    }
    fprintf(stderr,"}");
}

void printNodeJSON(node_t* node){
    if(node == NULL){
        printf("0");
        return;
    }
    printf("{");
    if(node->left == NULL && node->right == NULL){//leaf node
        char print_char = isprint(node->code)?node->code:' ';
        if(print_char == '\''){
            printf("\"char\":\"\\'\",");
        }else{
            printf("\"char\":\"%c\",",print_char);
        }
        printf("\"cnt\":%zu,\"code\":%d",node->cnt,(uint8_t)node->code);
    }else{//stem node
        printf("\"cnt\":%zu",node->cnt);
        printf(",\"left\":");
        printNodeJSON(node->left);
        printf(",\"right\":");
        printNodeJSON(node->right);
    }
    printf("}");
}


//tally the bits
node_t** tally(char* str, size_t size){//returns a ll of nodes
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

node_t* construct_huffman_tree(char* buff, size_t size){
    node_t** nodes = tally(buff,size);
    
    //printint nodes
    for(size_t i = 0; i < 256; i++){
        fprintNode(nodes[i]);
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
        fprintNode(heap[i]);
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


/*
typedef struct {
    bitfield_t* field;
    uint8_t bits[32];
    size_t size;
} cell_t;

uint8_t bitfield_set(uint8_t field, uint8_t offset, uint8_t val){
    field &= ~(1<<offset);
    field |= val<<offset;
    return field;
}

void write_bit(uint8_t* buff, size_t pos, uint8_t val){
    size_t offset = pos/8;
    uint8_t bits = (uint8_t)(pos-offset*8);
    buff[offset] = bitfield_set(buff[offset],bits,val);
}

uint8_t get_bit(uint8_t* buff, size_t pos){
    size_t offset = pos/8;
    uint8_t bits = (uint8_t)(pos-offset*8);
    return (buff[offset]>>bits)&1;
}

uint8_t* clone_buff(uint8_t* buff, size_t buffsize){
    uint8_t* buff1 = malloc(buffsize);
    for(size_t i = 0; i < buffsize; i++){
        buff1[i] = buff[i];
    }
    return buff1;
}

void copy_buff(uint8_t* buff1, uint8_t* buff2, size_t buffsize){
    for(size_t i = 0; i < buffsize; i++){
        buff2[i] = buff1[i];
    }
}*/





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
            //printf("%d",get_bit(testarr/*cell.bits*/,i));
            printf("%d",get_bit(cell.bits,i));
        }
        printf("}\n");
    }
    printf("]\n");
}

void construct_table_kernel(node_t* tree, cell_t* table, uint8_t* buff, size_t buffpos, size_t buffsize){//buff records the bit sequence
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

/*cell_t* construct_table(node_t* tree){
    cell_t* table = malloc(256*sizeof(cell_t));
    uint8_t* buff = malloc(32);
    construct_table_kernel(tree,table,buff,0,32);
    free(buff);
    return table;
}*/

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
}


bitfield_t* pack_table(cell_t* table){
    size_t max_size = 0;
    for(size_t i = 0; i < 256; i++){
        cell_t cell = table[i];
        max_size = cell.size > max_size ? cell.size : max_size;
    }
    //the first 2 bytes are for max cell size
    //initializing the field
    bitfield_t* field = bitfield_construct(256*sizeof(cell_t));
    
    bitfield_append_bits(&field,&max_size,16);//copy 16 bit
    for(size_t i = 0; i < 256; i++){
        cell_t cell = table[i];
        if(cell.size == 0){
            bitfield_append_bit(&field,0);
        }else{
            bitfield_append_bit(&field,1);
            bitfield_append_bits(&field,&cell.size,max_size);//copy the size
            bitfield_append_bits(&field,cell.bits,cell.size);//copy the size
        }
    }
    return field;
}


int main(int argc, char** argv){
    if(argc != 2)
        fprintf(stderr,"Usage: ./a.out filename\n");
    
    char* fname = argv[1];
    fprintf(stderr,"Reading the file: %s\n",fname);
    
    
    FILE *file = fopen(fname, "rb");
    fseek(file, 0, SEEK_END);
    size_t fsize = ftell(file);
    fseek(file, 0, SEEK_SET);  /* same as rewind(f); */

    char *buff = malloc(fsize);
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
    
    uint8_t* result = malloc(256*sizeof(cell_t));
    //write a compressed table as the header
    for(){
        
    }
    
    free_node_recursive(tree);
}




//not using it anymore but kinda hesitant to delete it
//qick sort
void sort_nodes(node_t** nodes, size_t start, size_t end){
    if(end - start <= 1){
        return;
    }
    node_t* piv_node = nodes[start];
    size_t piv_val = piv_node->cnt;
    size_t piv_pos = start;
    for(size_t i = start+1; i < end; i++){
        if(nodes[i]->cnt < piv_val){
            nodes[piv_pos] = nodes[i];
            piv_pos++;
            nodes[i] = nodes[piv_pos];
        }//else just progress the pointer
    }//now left of the piv will be lower, right will be higher in value
    nodes[piv_pos] = piv_node;
    sort_nodes(nodes,start,piv_pos);
    sort_nodes(nodes,piv_pos+1,end);
}




