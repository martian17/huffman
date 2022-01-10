#include "all.h"
#include "buffer.h"

#define maskleft(n) 255>>(8-(n))
#define maskright(n) 255<<(8-(n))

buffer_t* buffer_construct(size_t size){
    buffer_t* field = malloc(sizeof(buffer_t));
    field->size = size;
    field->offset_bytes = 0;
    field->offset_bits = 0;
    field->buff = calloc(size,1);
    return field;
}

buffer_t* buffer_from(uint8_t* buff, size_t size, size_t offset_bytes, uint8_t offset_bits){
    buffer_t* field = malloc(sizeof(buffer_t));
    field->size = size;
    field->offset_bytes = offset_bytes;
    field->offset_bits = offset_bits;
    field->buff = buff;
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


void buffer_append_bit(buffer_t* field, uint8_t bit){
    field->buff[field->offset_bytes] |= bit << field->offset_bits;
    //advance
    field->offset_bits++;
    field->offset_bytes += field->offset_bits/8;
    field->offset_bits %= 8;
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
        //do nothing
    }else if(offset_bits+remaining <= 8){//the rest fits in the byte
        buff1[offset_bytes] |= (buff2[i]<<offset_bits)&maskleft(offset_bits+remaining);
        //it will need to be ticked even if no overflow
        if(offset_bits+remaining == 8)offset_bytes++;
    }else{//doesn't fit in the byte
        buff1[offset_bytes] |= buff2[i]<<offset_bits;
        offset_bytes++;
        buff1[offset_bytes] |= (buff2[i]>>(8-offset_bits))&maskleft(offset_bits+remaining-8);
    }
    field->offset_bits = (offset_bits+remaining)%8;
    field->offset_bytes = offset_bytes;
}


//read methods
//advanes the bit pointer by default
uint8_t buffer_read_bit(buffer_t* field){
    uint8_t bits = field->offset_bits;
    size_t bytes = field->offset_bytes;
    uint8_t result = (field->buff[bytes]>>bits)&1;
    bits++;
    field->offset_bytes += bits/8;
    field->offset_bits = bits%8;
    return result;
}
void buffer_read_bits(buffer_t* field, uint8_t* buff2, size_t length){
    uint8_t bits = field->offset_bits;
    size_t bytes = field->offset_bytes;
    uint8_t* buff = field->buff;
    size_t i = 0;
    for(; i < length/8; i++){
        uint8_t byte = 0;
        byte |= buff[bytes]>>bits;
        bytes++;
        byte |= buff[bytes]<<(8-bits);
        buff2[i] = byte;
    }
    uint8_t remaining = length%8;
    if(remaining == 0){
        //do nothing
    }else if(remaining+bits <= 8){//firs in the current byte
        uint8_t byte = 0;
        byte |= (buff[bytes]>>bits)&maskleft(remaining);
        buff2[i] = byte;
        //it will need to be ticked even if no overflow
        if(bits+remaining == 8)bytes++;
    }else{//doesn't fir in a single byte
        uint8_t byte = 0;
        byte |= (buff[bytes]>>bits);
        bytes++;
        byte |= (buff[bytes]<<(8-bits))&maskleft(remaining);
        buff2[i] = byte;
    }
    field->offset_bits = (bits+remaining)%8;
    field->offset_bytes = bytes;
}





void buffer_print(buffer_t* field, size_t rowsize){
    size_t bytes = field->offset_bytes;
    uint8_t bits = field->offset_bits;
    for(size_t i = 0; i < bytes; i++){
        uint8_t b = field->buff[i];
        for(uint8_t i = 0; i < 8; i++){
            printf("%d",bitfield_get(b,i));
        }
        printf(" ");
        if((i+1)%rowsize == 0)printf("\n");
    }
    for(uint8_t i = 0; i < bits; i++){
        printf("%d",bitfield_get(field->buff[bytes],i));
    }
    printf("\n");
}














//generic bitwise util functions


uint8_t bitfield_set(uint8_t field, uint8_t offset, uint8_t val){
    field &= ~(1<<offset);
    field |= val<<offset;
    return field;
}

void buff_set_bit(uint8_t* buff, size_t pos, uint8_t val){
    size_t offset = pos/8;
    uint8_t bits = (uint8_t)(pos-offset*8);
    buff[offset] = bitfield_set(buff[offset],bits,val);
}

uint8_t bitfield_get(uint8_t field, uint8_t offset){
    return (field>>offset)&1;
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

void buff_print(uint8_t* buff, size_t offset){
    for(size_t i = 0; i < offset; i++){
        printf("%d",buff_get_bit(buff,i));
    }
}


