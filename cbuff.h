#ifndef __CBUFF_H__
#define __CBUFF_H__

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct CBUFF_STRUCT{
    volatile uint16_t head;
    volatile uint16_t tail;
    volatile uint16_t count;
    volatile uint16_t capacity;
    volatile uint8_t *data;
}CBUFF_Struct;

typedef CBUFF_Struct *CBUFF_Handle;

CBUFF_Handle CBUFF_construct(volatile CBUFF_Struct *cbuff, volatile uint8_t *data, uint16_t size);
void CBUFF_put(CBUFF_Handle handle, uint8_t data);
uint8_t CBUFF_get(CBUFF_Handle handle);
uint8_t CBUFF_peek(CBUFF_Handle handle);
int CBUFF_isFull(CBUFF_Handle hanle);
int CBUFF_isEmpty(CBUFF_Handle handle);


typedef struct CBUFF_OBJ_STRUCT{
    volatile void *obj;
    volatile uint16_t head;
    volatile uint16_t tail;
    volatile uint16_t capacity;
    volatile uint16_t count;
    volatile uint16_t obj_size;
} CBUFF_OBJ_Struct;

typedef CBUFF_OBJ_Struct *CBUFF_OBJ_Handle;

#ifdef	__cplusplus
}
#endif


#endif