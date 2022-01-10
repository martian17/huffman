#include "buffer.h"

buffer_t* buffer_construct(size_t size){
    buffer_t* field = malloc(sizeof(buffer_t));
    field->size = size;
    field->offset_bytes = 0;
    field->offset_bits = 0;
    field->buff = malloc(size);
    return field;
}

void buffer_destruct(buffer_t* field){
    free(field->buff);
    free(field);
}

void buffer_resize(buffer_t* field, size_t size){
    field->size = size;
    field->buff = realloc(field->buff,size);
}

void buffer_append_bits(buffer_t* field, uint8_t* buff2, size_t length){//length is the size of bits to be appended
    size_t offset_bytes = field->offset_bytes;
    uint8_t offset_bits = field->offset_bits;
    uint8_t* buff1 = field->buff;
    size_t i = 0;
    for(; i < length/8; i++){//full bits from the buff2
        //left segment
        buff1[offset_bytes] |= buff2[i]<<offset_bits;
        offset_bytes++;
        //right segment
        buff1[offset_bytes] |= buff2[i]>>(8-offset_bits);
    }
    size_t remaining = length%8;
    if(remaining == 0){
        return;
    }else if(offset_bits+remaining <= 8){//the rest fits in the byte
        buff1[offset_bytes] |= (buff2[i]<<offset_bits)&(255>>(8-offset_bits-remaining))
    }else{//doesn't fit in the byte
        buff1[offset_bytes] |= buff2[i]<<offset_bits;
        offset_bytes++;
        buff1[offset_bytes] |= (buff2[i]>>(8-offset_bits))&(255>>(offset_bits+remaining-8));
    }
    field->offset_bits = (offset_bits+remaining)%8;
    field->offset_bytes = offset_bytes;
}

void buffer_append_bit(buffer_t* field, uint8_t bit){
    buff1[field->offset_bytes] |= bit << field->offset_bits;
    //advance
    field->offset_bits++;
    field->offset_bytes += field->offset_bits/8;
    field->offset_bits %= 8;
}














//generic bitwise util functions


uint8_t bitfield_set(uint8_t field, uint8_t offset, uint8_t val){
    field &= ~(1<<offset);
    field |= val<<offset;
    return field;
}

void buff_write_bit(uint8_t* buff, size_t pos, uint8_t val){
    size_t offset = pos/8;
    uint8_t bits = (uint8_t)(pos-offset*8);
    buff[offset] = bitfield_set(buff[offset],bits,val);
}

uint8_t buff_get_bit(uint8_t* buff, size_t pos){
    size_t offset = pos/8;
    uint8_t bits = (uint8_t)(pos-offset*8);
    return (buff[offset]>>bits)&1;
}

uint8_t* buff_clone(uint8_t* buff, size_t buffsize){
    uint8_t* buff1 = malloc(buffsize);
    for(size_t i = 0; i < buffsize; i++){
        buff1[i] = buff[i];
    }
    return buff1;
}

void buff_copy(uint8_t* buff1, uint8_t* buff2, size_t buffsize){
    for(size_t i = 0; i < buffsize; i++){
        buff2[i] = buff1[i];
    }
}


