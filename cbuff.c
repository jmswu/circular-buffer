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

uint8_t CBUFF_peek(CBUFF_Handle handle){

    /* check handle */
    if (handle == 0) return 0;

    return handle->data[handle->tail];
}

int CBUFF_isFull(CBUFF_Handle handle){

    int result = 0;

    CBUFF_CRITICAL_SECTION_BEGIN();
    if (handle->count >= handle->capacity){
        result = 1;
    }else{
        result = 0;
    }
    CBUFF_CRITICAL_SECTION_END();

    return result;

}

int CBUFF_isEmpty(CBUFF_Handle handle){

    int result = 0;

    CBUFF_CRITICAL_SECTION_BEGIN();
    if (handle->count == 0){
        result = 1;
    }else{
        result = 0;
    }
    CBUFF_CRITICAL_SECTION_END();

    return result;

}

CBUFF_OBJ_Handle CBUFF_OBJ_construct(volatile CBUFF_OBJ_Struct *cbuff, volatile uint8_t *data, uint16_t obj_size, uint16_t capacity){

    /* make sure pointers are valid */
    if (cbuff == 0) return 0;
    if (data == 0) return 0;

    cbuff->capacity = capacity;
    cbuff->count = 0;
    cbuff->head = 0;
    cbuff->tail = 0;
    cbuff->data = data;
    cbuff->obj_size = obj_size;

    return (CBUFF_OBJ_Handle)cbuff;
}

void CBUFF_OBJ_put(CBUFF_OBJ_Handle handle, void *obj){

    /* make sure pointers are valid */
    if (handle == 0) return;
    if (obj == 0) return;

    /* offset location for the destination data */
    uint16_t offset = 0;
    offset = handle->head * handle->obj_size;

    uint8_t *pOjb = (uint8_t *)obj;
    uint8_t *pBuff = (uint8_t *)(handle->data);
    
    /* copy data into the ring buffer */
    memcpy(pBuff + offset, pOjb, handle->obj_size);

    /* point to next location */
    handle->head++;

    /* check for overflow */
    if (handle->head >= handle->capacity){
        handle->head = 0;
    }

    /* increase counter */
    CBUFF_CRITICAL_SECTION_BEGIN();
    handle->count++;
    CBUFF_CRITICAL_SECTION_END();

    return;
}

void CBUFF_OBJ_get(CBUFF_OBJ_Handle handle, void *obj){

    /* pointer check */
    if (handle == 0) return;
    if (obj == 0) return;

    /* offset location for the object */
    uint16_t offset = 0;
    offset = handle->obj_size * handle->tail;

    uint8_t *pObj = (uint8_t *)obj;
    uint8_t *pBuff = (uint8_t *)(handle->data);

    /* copy the data from buffer to the external object */
    memcpy(pObj, pBuff + offset, handle->obj_size);

    /* point to next location */
    handle->tail++;

    /* check for overflow */
    if (handle->tail >= handle->capacity){
        handle->tail = 0;
    }

    /* increase counter */
    CBUFF_CRITICAL_SECTION_BEGIN();
    handle->tail--;
    CBUFF_CRITICAL_SECTION_END();

    return;
}

int CBUFF_OBJ_isFull(CBUFF_OBJ_Handle handle){

    /* pointer check */
    if (handle == 0) return 1;

    int result = 0;

    CBUFF_CRITICAL_SECTION_BEGIN();
    if (handle->count == handle->capacity){
        result = 1;
    }else{
        result = 0;
    }
    CBUFF_CRITICAL_SECTION_END();

    return result;
}


int CBUFF_OBJ_isEmpty(CBUFF_OBJ_Handle handle){

    /* pointer check */
    if (handle == 0) return 1;

    int result = 0;

    CBUFF_CRITICAL_SECTION_BEGIN();
    if (handle->count == 0){
        result = 1;
    }else{
        result = 0;
    }
    CBUFF_CRITICAL_SECTION_END();

    return result;
}