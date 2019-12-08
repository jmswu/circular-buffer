#ifndef __CBUFF_H__
#define __CBUFF_H__

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>

#define CBUFF_CRITICAL_SECTION_BEGIN()      do{/* user code here */}while(0)
#define CBUFF_CRITICAL_SECTION_END()        do{/* user code here */}while(0)


typedef struct CBUFF_STRUCT{
    volatile uint16_t head;                 /* pointing to the head of the ring buffer      */
    volatile uint16_t tail;                 /* pointing to the tail of the ring buffer      */
    volatile uint16_t count;                /* number of byte currented in the ring buffer  */
    volatile uint16_t capacity;             /* the maximum of byte allowed in the buffer    */
    volatile uint8_t *data;                 /* pointer to the buffer                        */
}CBUFF_Struct;

typedef CBUFF_Struct *CBUFF_Handle;

/* Construct the initialize the ring buffer
 * @param volatile CBUFF_Struct *cbuff      - pointer to the ring buffer structure
 * @param volatile uint8_t *data            - pointer to the ring buffer memory location
 * @param uint16_t size                     - the maximum capacity of the ring buffer
 * @return CBUFF_Handle                     - handle to ring buffer
 */
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