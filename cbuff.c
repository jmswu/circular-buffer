#include "cbuff.h"

CBUFF_Handle CBUFF_construct(volatile CBUFF_Struct *cbuff, volatile uint8_t *data, uint16_t size){
    cbuff->capacity = size;
    cbuff->count = 0;
    cbuff->data = data;
    cbuff->head = 0;
    cbuff->tail=0;
    return (CBUFF_Handle)cbuff;
}