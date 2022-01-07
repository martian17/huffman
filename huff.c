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
        printf("cnt:%ld,code:%d",node->cnt,(uint8_t)node->code);
    }else{//stem node
        printf("cnt:%ld",node->cnt);
        printf(",left:");
        printNode(node->left);
        printf(",right:");
        printNode(node->right);
    }
    printf("}");
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
    sort_nodes((node_t**)table,0,256);
    return (node_t**)table;
}

ll_node_t* construct_tree(ll_node_t* head, size_t max_val){
    ll_node_t* next = head->next;
    if(next == NULL){
        return head;
    }
    node_t* head_node = head->content;
    node_t* next_node = next->content;
    //unite what needs to be united
    size_t cnt1 = head_node->cnt;
    size_t cnt2 = next_node->cnt;
    while(cnt1 < max_val){
        if(cnt1 > cnt2){//unite what's on the right
            next = construct_tree(next,cnt1);
            next_node = next->content;
            cnt2 = next_node->cnt;
        }
        if(cnt2 < max_val){
            //merge head and next
            node_t* node1 = malloc(sizeof(node_t));
            node1->left = head_node;
            node1->right = next_node;
            node1->cnt = cnt1+cnt2;
            node1->code = 0;
            head->content = node1;
            head->next = next->next;
            free(next);
            
            //parity
            
        }
    }
    return head;
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
    
    //constructing a tree
    
    size_t i = 0;
    for(; i < 256; i++){
        node_t* node = nodes[i];
        if(node->cnt != 0){
            break;
        }else{
            //free everything
            free(node);
        }
    }
    
    if(i == 256){
        fprintf(stderr,"empty file");
        exit(1);
    }
    
    //convert this to linked list
    ll_node_t* head = malloc(sizeof(ll_node_t));
    head->content = nodes[i];
    ll_node_t* tail = head;
    i++;
    
    for(; i < 256; i++){
        tail->next = malloc(sizeof(ll_node_t));
        tail = tail->next;
        tail->content = nodes[i];
    }
    tail->next = NULL;
    
    ll_node_t* llnode = construct_tree(head,SIZE_MAX);
    if(llnode->next != NULL){
        fprintf(stderr, "error constructing a huffman tree, extra element in the ll\n");
        while(llnode != NULL){
            printNode(llnode->content);
            printf("\n\n");
            llnode = llnode->next;
        }
        exit(1);
    }
    node_t* tree = llnode->content;
    free(llnode);
    printNode(tree);
    printf("\n");
    
    
    
    /*
    
    node_t* root = nodes[i];
    i++;
    
    for(; i < 256; i++){
        node_t* node = nodes[i];
        size_t score = node->cnt;
        if(score < root->score){
            
        }
        if(root->score < score){//merge with the root
            node_t* node1 = malloc(sizeof(node_t));
            node1->left = root;
            node1->right = node;
            node1->cnt = score+root->cnt;
            root = node1;
        }else{//merge the two kids
            
        }
    }
    */
    
    
}





