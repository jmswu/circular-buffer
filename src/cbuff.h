#ifndef __CBUFF_H__
#define __CBUFF_H__

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>

/* Use fast modulo 2 arithmetic. If enabling this 
 * Its capacity is **REQUIRED** to be a power of two.
 */
//#define CBUFF_USE_FAST_MODULO_TWO_ARITHEMTIC

#ifdef CBUFF_USE_FAST_MODULO_TWO_ARITHEMTIC
#warning "Using fast modulo two arithemtic, buffer capacity is **REQUIRED** to be a power of two."
#endif

/*! User need to insert codes that apply to the archetechure 
 *! that will disable global interrupt
 */
#define CBUFF_CRITICAL_SECTION_BEGIN()      do{/* user code here */}while(0)
#define CBUFF_CRITICAL_SECTION_END()        do{/* user code here */}while(0)

typedef struct CBUFF_STRUCT{
    volatile uint16_t head;                 /* pointing to the head of the ring buffer      */
    volatile uint16_t tail;                 /* pointing to the tail of the ring buffer      */
    volatile uint16_t count;                /* number of byte currented in the ring buffer  */
    volatile uint16_t capacity;             /* the maximum of byte allowed in the buffer    */
    volatile unsigned overFlowCount;        /* overflow counts                              */
    volatile unsigned underFlowCount;       /* underflow counts                             */
#ifdef CBUFF_USE_FAST_MODULO_TWO_ARITHEMTIC
    volatile uint16_t mask_fast_arithemtic; /* mask used in fast modulo two arithemtic      */
#endif
    volatile uint8_t *data;                 /* pointer to the buffer                        */
}CBUFF_Struct;

typedef CBUFF_Struct *CBUFF_Handle;

uint32_t CBUFF_getVerNumber(void);

/*! \brief Construct the initialize the ring buffer
 *! \param volatile CBUFF_Struct *cbuff      - pointer to the ring buffer structure
 *! \param volatile uint8_t *data            - pointer to the ring buffer memory location
 *! \param uint16_t size                     - the maximum capacity of the ring buffer
 *! \return CBUFF_Handle                     - handle to ring buffer
 */
CBUFF_Handle CBUFF_construct(volatile CBUFF_Struct * const cbuff, volatile uint8_t * const data, const uint16_t size);

/*! \brief  Put one byte into the circular buffer
 *!         If buffer is full, no more data will be put into
 *!         the buffer until there is more space
 *! \param CBUFF_Handle handle               - circular buffer handler
 *! \param uint8_t data                      - one byte that need to be put into the buffer
 *! \return                                  - void
 */
void CBUFF_put(const CBUFF_Handle handle, const uint8_t data);

/*! \brief  Put one block of data into the circular buffer
 *! \param CBUFF_Handle handle               - circular buffer handler
 *! \param uint8_t *data                     - pointer to the data
 *! \param uint8_t size                      - size of the data
 *! \return                                  - 1 = successful, 0 = failed
 */
uint8_t CBUFF_put_block(const CBUFF_Handle handle, const uint8_t * const data, const uint8_t size);

/*! \brief Get one byte from the circular buffer
 *!         If there is no more data in the buffer
 *!         It will always return 0 with no warnings
 *! \param CBUFF_Handle handle               - circular buffer handler
 *! \return uint8_t                          - one byte read from the circular buffer
 */
uint8_t CBUFF_get(const CBUFF_Handle handle);

/*! \brief Get one byte from the circular buffer without actually remove it from the ring buffer
 *!         It will return 0 if buffer is empty, with no warnings
 *! \param CBUFF_Handle handle               - circular buffer handler
 *! \return uint8_t                          - one byte read from the circular buffer
 */
uint8_t CBUFF_peek(const CBUFF_Handle handle);

/*! \brief Tell if the ring buffer is full
 *! \param CBUFF_Handle handle               - circular buffer handler
 *! \return int                              - 1 = is full, 0 = is not full
 */
int CBUFF_isFull(const CBUFF_Handle handle);

/*! \brief Tell if there is space in the ring buffer
 *! \param CBUFF_Handle handle               - circular buffer handler
 *! \return int                              - 1 = there is space, 0 = is full, no space left
 */
int CBUFF_isEmptySpaceAvailable(const CBUFF_Handle handle);

/*! \brief Tell if the ring buffer is empty
 *! \param CBUFF_Handle handle               - circular buffer handler
 *! \return int                              - 1 = is empty, 0 = is not empty
 */
int CBUFF_isEmpty(const CBUFF_Handle handle);

/*! \brief Tell if there is data in the ring buffer
 *! \param CBUFF_Handle handle               - circular buffer handler
 *! \return int                              - 1 = there is data, 0 = is empty, no data available
 */
int CBUFF_isDataAvailable(const CBUFF_Handle handle);

/*! \brief Return the number of free bytes in the buffer
 *! \param CBUFF_Handle handle               - circular buffer handler
 *! \return uint16_t                         - free byte
 */
uint16_t CBUFF_getNumOfFreeByte(const CBUFF_Handle handle);

/*! \brief Return the number of overflow/underflow counts
 *! \param CBUFF_Handle handle               - circular buffer handler
 *! \return unsigned                         - number of count
 */
unsigned CBUFF_getOverflowCounts(const CBUFF_Handle handle);
unsigned CBUFF_getUnderflowCounts(const CBUFF_Handle handle);

/*! Struture for constructing a object ring buffer
 */
typedef struct CBUFF_OBJ_STRUCT{
    volatile void *data;
    volatile uint16_t head;
    volatile uint16_t tail;
    volatile uint16_t capacity;
    volatile uint16_t count;
    volatile uint16_t obj_size;
    volatile uint16_t overflowCount;
    volatile uint16_t underflowCount;
} CBUFF_OBJ_Struct;

typedef CBUFF_OBJ_Struct *CBUFF_OBJ_Handle;

/*! \brief Construct the initialize the ring buffer
 *! \param volatile CBUFF_OBJ_Struct *cbuff      - pointer to the object ring buffer structure
 *! \param volatile uint8_t *data                - pointer to the ring buffer memory location
 *! \param uint16_t obj_size                     - size in byte of the object
 *! \param uint16_t capacity                     - Maximum number of object that can be stored in the buffer
 *! \return CBUFF_OBJ_Handle                     - handle to object ring buffer
 */
CBUFF_OBJ_Handle CBUFF_OBJ_construct(volatile CBUFF_OBJ_Struct *cbuff, volatile uint8_t *data, uint16_t obj_size, uint16_t capacity);

/*! \brief Put one object into the circular buffer
 *! \param CBUFF_OBJ_Handle handle           - object circular buffer handler
 *! \param void *obj                         - pointer to the object that need to be put into the buffer
 *! \return                                  - success = 1, failed = 0;
 */
int CBUFF_OBJ_put(CBUFF_OBJ_Handle handle, void *obj);

/*! \brief Get one object from the circular buffer
 *! \param CBUFF_OBJ_Handle handle           - circular buffer handler
 *! \param void *obj                         - pointer to the object that CBUFF_OBJ_get() write to
 *! \return                                  - success = 1, failed = 0;
 */
int CBUFF_OBJ_get(CBUFF_OBJ_Handle handle, void *obj);

/*! \brief Get one object from the circular buffer without actually remove it from the ring buffer
 *! \param CBUFF_OBJ_Handle handle           - circular buffer handler
 *! \param void *obj                         - pointer to the object that CBUFF_OBJ_get() write to
 *! \return                                  - success = 1, failed = 0;
 */
int CBUFF_OBJ_peek(CBUFF_OBJ_Handle handle, void *obj);


/*! \brief Tell if the ring buffer is full
 *! \param CBUFF_OBJ_Handle handle           - object circular buffer handler
 *! \return int                              - 1 = is full, 0 = is not full
 */
int CBUFF_OBJ_isFull(CBUFF_OBJ_Handle handle);

/*! \brief Tell if the ring buffer is empty
 *! \param CBUFF_OBJ_Handle handle           - object circular buffer handler
 *! \return int                              - 1 = is empty, 0 = is not empty
 */
int CBUFF_OBJ_isEmpty(CBUFF_OBJ_Handle handle);

/*! \brief Return the number of free bytes in the buffer
 *! \param CBUFF_OBJ_Handle handle           - circular buffer handler
 *! \return uint16_t                         - free byte
 */
uint16_t CBUFF_OBJ_getNumOfFreeSlot(CBUFF_OBJ_Handle handle);

/*! \brief Return the number of overflow/underflow counts
 *! \param CBUFF_Handle handle               - circular buffer handler
 *! \return uint16_t                         - number of count
 */
uint16_t CBUFF_OBJ_getOverflowCounts(const CBUFF_OBJ_Handle handle);
uint16_t CUBFF_OBJ_getUnderflowCounts(const CBUFF_OBJ_Handle handle);



#ifdef	__cplusplus
}
#endif


#endif