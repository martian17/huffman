#ifndef BUFFER_H
#define BUFFER_H

//buffer and their methods
typedef struct {
    size_t size;
    size_t offset_bytes;
    uint8_t offset_bits;
    uint8_t* buff;
} buffer_t;

buffer_t* buffer_construct(size_t size);
void buffer_destruct(buffer_t* field);
void buffer_resize(buffer_t* field, size_t size);
void buffer_append_bits(buffer_t* field, uint8_t* buff2, size_t length);
void buffer_append_bit(buffer_t* field, uint8_t bit);
void buffer_print(buffer_t* field, size_t rowsize);


//generic bitwise util functions
uint8_t bitfield_set(uint8_t field, uint8_t offset, uint8_t val);
void buff_set_bit(uint8_t* buff, size_t pos, uint8_t val);
uint8_t bitfield_get(uint8_t field, uint8_t offset);
uint8_t buff_get_bit(uint8_t* buff, size_t pos);
uint8_t* buff_clone(uint8_t* buff, size_t buffsize);
void buff_copy(uint8_t* buff1, uint8_t* buff2, size_t buffsize);

#endif