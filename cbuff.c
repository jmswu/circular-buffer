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

void CBUFF_put(CBUFF_Handle handle, uint8_t data){
    
    /* check handle */
    if (handle == 0) return;

    /* put data into the buffer */
    handle->data[handle->head++] = data;

    /* check for overflow */
    if (handle->head >= handle->capacity){
        handle->head = 0;
    }

    /* increase queue data count */
    CBUFF_CRITICAL_SECTION_BEGIN();
    handle->count++;
    CBUFF_CRITICAL_SECTION_END();

    return;
}

uint8_t CBUFF_get(CBUFF_Handle handle){

    /* check handle */
    if (handle == 0) return 0;

    /* get one byte */
    uint8_t data = 0;
    data = handle->data[handle->tail++];

    /* check for overflow */
    if (handle->tail >= handle->capacity){
        handle->tail = 0;
    }

    /* decrease queued data count */
    CBUFF_CRITICAL_SECTION_BEGIN();
    handle->count--;
    CBUFF_CRITICAL_SECTION_END();

    return data;
}