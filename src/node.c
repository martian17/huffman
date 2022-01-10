#include "all.h"
#include "node.h"

void node_destruct_recursive(node_t* node){
    if(node->left != NULL){
        node_destruct_recursive(node->left);
    }
    if(node->right != NULL){
        node_destruct_recursive(node->right);
    }
    free(node);
}

void node_print(node_t* node){
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
        node_print(node->left);
        printf(",right:");
        node_print(node->right);
    }
    printf("}");
}

void node_fprint(node_t* node){
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
        node_fprint(node->left);
        fprintf(stderr,",right:");
        node_fprint(node->right);
    }
    fprintf(stderr,"}");
}

void node_print_json(node_t* node){
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
        node_print_json(node->left);
        printf(",\"right\":");
        node_print_json(node->right);
    }
    printf("}");
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


