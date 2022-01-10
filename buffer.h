#include "all.h"


//buffer and their methods
typedef struct {
    size_t size;
    size_t offset_bytes;
    size_t offset_bits;
    uint8_t* buff;
} buffer_t;

buffer_t* buffer_construct;

void buffer_destruct(buffer_t* field);
void buffer_resize(buffer_t* field, size_t size);
void buffer_append_bits(buffer_t* field, uint8_t* buff2, size_t length);
void buffer_append_bit(buffer_t* field, uint8_t bit);