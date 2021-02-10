#include "cbuff.h"

#define VER_MAJOR   (0U)
#define VER_MINOR   (1U)
#define VER_PATCH   (3U)
#define VER_NUM     ((VER_MAJOR << 16) + (VER_MINOR << 8) + (VER_PATCH))

uint32_t CBUFF_getVerNumber(void){
    return VER_NUM;
}

CBUFF_Handle CBUFF_construct(volatile CBUFF_Struct *cbuff, volatile uint8_t *data, uint16_t size){

    /* make sure pointers are valid */
    if (cbuff == 0) return 0;
    if (data == 0) return 0;
    if (size == 0) return 0;

    memset((void *)cbuff, 0, sizeof(CBUFF_Struct));

    cbuff->capacity = size;
    cbuff->data = data;

#ifdef CBUFF_USE_FAST_MODULO_TWO_ARITHEMTIC
    /* calculate the mask use for the fast modulo 2 arithmetic */
    cbuff->mask_fast_arithemtic = cbuff->capacity - 1;
#endif

    return (CBUFF_Handle)cbuff;
}

void CBUFF_put(CBUFF_Handle handle, uint8_t data){
    
    /* check handle */
    if (handle == 0) return;

    /* chekc if full */
    if (CBUFF_isFull(handle)) {
        handle->overFlowCount++;
        return;
    }

    /* put data into the buffer */
    handle->data[handle->head++] = data;

#ifdef CBUFF_USE_FAST_MODULO_TWO_ARITHEMTIC
    /* check for overflow */
    handle->head = handle->head & handle->mask_fast_arithemtic;
#else
    /* check for overflow */
    if (handle->head >= handle->capacity){
        handle->head = 0;
    }
#endif


    /* increase queue data count */
    CBUFF_CRITICAL_SECTION_BEGIN();
    if (handle->count < handle->capacity){
        handle->count++;
    }
    CBUFF_CRITICAL_SECTION_END();

    return;
}

uint8_t CBUFF_put_block(CBUFF_Handle handle, uint8_t *data, uint8_t size){

    /* null check */
    if (handle == NULL) return 0;
    if (data == NULL) return 0;

    uint16_t free_space = CBUFF_getNumOfFreeByte(handle);

    /* make sure there is enought free space */
    if (free_space < size) return 0;

    /* put data into the buffer */
    for (uint8_t i = 0; i < size; i++){
        CBUFF_put(handle, data[i]);
    }

    return 1;
}

uint8_t CBUFF_get(CBUFF_Handle handle){

    /* check handle */
    if (handle == 0) return 0;

    if (CBUFF_isEmpty(handle)) {
        handle->underFlowCount++;
        return 0;
    }

    /* get one byte */
    uint8_t data = 0;
    data = handle->data[handle->tail++];

#ifdef CBUFF_USE_FAST_MODULO_TWO_ARITHEMTIC
    /* check for overflow */
    handle->tail = handle->tail & handle->mask_fast_arithemtic;
#else
    /* check for overflow */
    if (handle->tail >= handle->capacity){
        handle->tail = 0;
    }
#endif

    /* decrease queued data count */
    CBUFF_CRITICAL_SECTION_BEGIN();
    if (handle->count > 0){
        handle->count--;
    }
    CBUFF_CRITICAL_SECTION_END();

    return data;
}

uint8_t CBUFF_peek(CBUFF_Handle handle){

    /* check handle */
    if (handle == 0) return 0;

    if (CBUFF_isEmpty(handle))
    {
        return 0;
    }
    else
    {
        return handle->data[handle->tail];
    }
}

int CBUFF_isFull(CBUFF_Handle handle){

    const int FULL = 1;
    const int NOT_FULL = 0;

    if (handle == 0) return FULL;

    int isFull = NOT_FULL;

    CBUFF_CRITICAL_SECTION_BEGIN();
    if (handle->count >= handle->capacity){
        isFull = FULL;
    }else{
        isFull = NOT_FULL;
    }
    CBUFF_CRITICAL_SECTION_END();

    return isFull;

}

int CBUFF_isEmpty(CBUFF_Handle handle){

    const int EMPTY = 1;
    const int NOT_EMPTY = 0;

    if (handle == 0) return EMPTY;

    int isEmpty = NOT_EMPTY;

    CBUFF_CRITICAL_SECTION_BEGIN();
    if (handle->count == 0){
        isEmpty = EMPTY;
    }else{
        isEmpty = NOT_EMPTY;
    }
    CBUFF_CRITICAL_SECTION_END();

    return isEmpty;

}

uint16_t CBUFF_getNumOfFreeByte(CBUFF_Handle handle){
    uint16_t retval = 0;

    if (handle == 0) return retval;

    CBUFF_CRITICAL_SECTION_BEGIN();
    retval = handle->capacity - handle->count;
    CBUFF_CRITICAL_SECTION_END();
    return retval;
}

unsigned CBUFF_getOverflowCounts(CBUFF_Handle handle) {
    if (handle == 0) return ~0;
    return handle->overFlowCount;
}

unsigned CBUFF_getUnderflowCounts(CBUFF_Handle handle) {
    if (handle == 0) return ~0;
    return handle->underFlowCount;
}

CBUFF_OBJ_Handle CBUFF_OBJ_construct(volatile CBUFF_OBJ_Struct *cbuff, volatile uint8_t *data, uint16_t obj_size, uint16_t capacity){

    /* make sure pointers are valid */
    if (cbuff == 0) return 0;
    if (data == 0) return 0;
    if (obj_size == 0) return 0;
    if (capacity == 0) return 0;

    cbuff->capacity = capacity;
    cbuff->count = 0;
    cbuff->head = 0;
    cbuff->tail = 0;
    cbuff->data = data;
    cbuff->obj_size = obj_size;

    return (CBUFF_OBJ_Handle)cbuff;
}

int CBUFF_OBJ_put(CBUFF_OBJ_Handle handle, void *obj){

    const int SUCCESS = 1;
    const int FAILED = 0;

    /* make sure pointers are valid */
    if (handle == 0) return FAILED;
    if (obj == 0) return FAILED;
    
    if (CBUFF_OBJ_isFull(handle)) return FAILED;

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
    if (handle->count < handle->capacity){
        handle->count++;
    }
    CBUFF_CRITICAL_SECTION_END();

    return SUCCESS;
}

int CBUFF_OBJ_get(CBUFF_OBJ_Handle handle, void *obj){

    const int SUCCESS = 1;
    const int FAILED = 0;

    /* pointer check */
    if (handle == 0) return FAILED;
    if (obj == 0) return FAILED;
    if (CBUFF_OBJ_isEmpty(handle)) return FAILED;

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
    if (handle->count > 0){
        handle->count--;
    }
    CBUFF_CRITICAL_SECTION_END();

    return SUCCESS;
}

int CBUFF_OBJ_peek(CBUFF_OBJ_Handle handle, void *obj){

    const int SUCCESS = 1;
    const int FAILED = 0;

    /* pointer check */
    if (handle == 0) return FAILED;
    if (obj == 0) return FAILED;

    if (CBUFF_OBJ_isEmpty(handle)) return FAILED;

    /* offset location for the object */
    uint16_t offset = 0;
    offset = handle->obj_size * handle->tail;

    uint8_t *pObj = (uint8_t *)obj;
    uint8_t *pBuff = (uint8_t *)(handle->data);

    /* copy the data from buffer to the external object */
    memcpy(pObj, pBuff + offset, handle->obj_size);

    return SUCCESS;
}

int CBUFF_OBJ_isFull(CBUFF_OBJ_Handle handle){

    /* pointer check */
    if (handle == 0) return 1;

    const int FULL = 1;
    const int NOT_FULL = 0;

    int isFull = NOT_FULL;

    CBUFF_CRITICAL_SECTION_BEGIN();
    if (handle->count == handle->capacity){
        isFull = FULL;
    }else{
        isFull = NOT_FULL;
    }
    CBUFF_CRITICAL_SECTION_END();

    return isFull;
}


int CBUFF_OBJ_isEmpty(CBUFF_OBJ_Handle handle){

    /* pointer check */
    if (handle == 0) return 1;

    const int EMPTY = 1;
    const int NOT_EMPTY = 0;

    int isEmpty = NOT_EMPTY;

    CBUFF_CRITICAL_SECTION_BEGIN();
    if (handle->count == 0){
        isEmpty = EMPTY;
    }else{
        isEmpty = NOT_EMPTY;
    }
    CBUFF_CRITICAL_SECTION_END();

    return isEmpty;
}

uint16_t CBUFF_OBJ_getNumOfFreeSlot(CBUFF_OBJ_Handle handle){
    uint16_t retval = 0;

    if (handle == 0) return retval;

    CBUFF_CRITICAL_SECTION_BEGIN();
    retval = handle->capacity - handle->count;
    CBUFF_CRITICAL_SECTION_END();
    return retval;
}