#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

typedef struct{
    char code;
    size_t cnt;
    void* left;
    void* right;
} node_t;

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

typedef struct{
    void* content;
    void* next;
} ll_node_t;

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

//tally the bits
node_t** tally(char* str, size_t size){//returns a ll of nodes
    //size_t table[256];// = calloc(256, sizeof(size_t));
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
    //sort the tally from lower to hight
    //sort_nodes((node_t**)table,0,256);
    return (node_t**)table;
}

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
    //fprintf(stderr,"tail/2: %ld\n",tail/2);
    //fprintf(stderr,"pppppp: %ld\n",heap[tail/2]);
    node_t* parent = heap[tail/2];
    //fprintNode(node);
    //fprintf(stderr,"\n");
    //fprintNode(parent);
    //fprintf(stderr,"\n");
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
    
    for(size_t i = 0; i < fsize; i++){
        printf("%c",buff[i]);
    }
    
    
    node_t** nodes = tally(buff,fsize);
    
    for(size_t i = 0; i < 256; i++){
        node_t* node = nodes[i];
        printf("letter: \"%c\"\n",node->code);
        printf("code:   %d \n",(uint8_t)node->code);
        printf("cnt:    %ld\n",node->cnt);
        printf("\n");
    }
    
    
    //convert this to a inary heap
    node_t** heap = minheap_construct(256);
    size_t tail = 0;
    
    for(size_t i = 0; i < 256; i++){
        //fprintf(stderr,"!%d\n",i);
        minheap_push(heap,&tail,nodes[i]);
    }
    fprintf(stderr,"tail: %zu\n",tail);
    fprintf(stderr,"Now printing heap !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    
    
    //nodes no longer needed
    free(nodes);
    
    for(size_t i = 0; i < 256; i++){
        if(heap[1]->cnt != 0){
            break;
        }
        node_t* node = minheap_pop(heap,&tail);
        free(node);
    }
    fprintf(stderr,"tail: %zu\n",tail);
    
    for(size_t i = 0; i <= tail; i++){
        fprintNode(heap[i]);
        fprintf(stderr,"\n");
    }
    
    fprintf(stderr,"\n");
    
    /*while(tail != 1){//until there is a single element left in the heap
        node_t* node = minheap_pop(heap,&tail);
        fprintNode(node);
        fprintf(stderr,"\n");
    }*/
    
    
    //constructing a tree
    while(tail != 1){//until there is a single element left in the heap
        node_t* node1 = minheap_pop(heap,&tail);
        /*fprintNode(node1);
        fprintf(stderr,"\n");*/
        node_t* node2 = minheap_pop(heap,&tail);
        node_t* node = malloc(sizeof(node_t));
        node->left = node1;
        node->right = node2;
        node->cnt = node1->cnt+node2->cnt;
        minheap_push(heap,&tail,node);
    }
    node_t* tree = heap[1];
    minheap_destruct(heap);
    //now we have a clean tree to work with
    printNode(tree);
    printf("\n");
    free(tree);
}





