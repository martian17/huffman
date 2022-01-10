#ifndef NODE_H
#define NODE_H

typedef struct{
    uint8_t code;
    size_t cnt;
    void* left;
    void* right;
} node_t;

void node_destruct_recursive(node_t* node);
void node_print(node_t* node);
void node_fprint(node_t* node);
void node_print_json(node_t* node);
void sort_nodes(node_t** nodes, size_t start, size_t end);

#endif