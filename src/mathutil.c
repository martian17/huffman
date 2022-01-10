#include "all.h"
#include "mathutil.h"


size_t log2ceil(size_t num){
    //count the number of leading zeros
    for(uint8_t i = 0; i < sizeof(size_t)*8; i++){
        if(!(num>>i)){
            return i;
        }
    }
    return sizeof(size_t)*8;
}