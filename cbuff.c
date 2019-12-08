#include "cbuff.h"

CBUFF_Handle CBUFF_construct(volatile CBUFF_Struct *cbuff, volatile uint8_t *data, uint16_t size){

    /* make sure pointers are valid */
    if (cbuff == 0) return 0;
    if (data == 0) return 0;

    cbuff->capacity = size;
    cbuff->count = 0U;
    cbuff->data = data;
    cbuff->head = 0U;
    cbuff->tail=0U;
    return (CBUFF_Handle)cbuff;
}