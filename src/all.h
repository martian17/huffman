#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>


typedef struct{
    uint8_t code;
    size_t cnt;
    void* left;
    void* right;
} node_t;