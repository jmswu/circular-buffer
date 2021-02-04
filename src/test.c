#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cbuff.h"
#include "Unity/unity.h"

void TEST_HANDLE_CREATION(void);
void TEST_ADD_DATA(void);
void TEST_ADD_DATA_BUFFER_FULL(void);
void TEST_ADD_DATA_OVERFLOW(void);
void TEST_REMOVE_DATA(void);
void TEST_REMOVE_DATA_BUFFER_EMPTY(void);
void TEST_REMOVE_DATA_UNDERFLOW(void);
void TEST_PEEK(void);
void TEST_PEEK_UNDERFLOW(void);
void TEST_GET_FREE_BYTE(void);
void TEST_RANDOM_DATA(void);

void TEST_OBJ_HANDLE_CREATION(void);

int printEndingMessage(void);

typedef struct {
    uint8_t data1;
    uint16_t data2;
    uint32_t data3;
}testObjType;

int main(int argc, char ** argv){

    /* dummp code to get rid of warnings */
    argc++;
    argv++;

    TEST_HANDLE_CREATION();
    TEST_ADD_DATA();
    TEST_ADD_DATA_BUFFER_FULL();
    TEST_ADD_DATA_OVERFLOW();
    TEST_REMOVE_DATA();
    TEST_REMOVE_DATA_BUFFER_EMPTY();
    TEST_REMOVE_DATA_UNDERFLOW();
    TEST_PEEK();
    TEST_PEEK_UNDERFLOW();
    TEST_GET_FREE_BYTE();
    TEST_RANDOM_DATA();

    TEST_OBJ_HANDLE_CREATION();

    /* CBUFF_OBJ is not tested */

    return printEndingMessage();
}

int printEndingMessage(void)
{
    printf("-------------------------------------------------------------------------\n");
    printf("If you ONLY see this and no other message, it means all tests are passed!\n");
    printf("-------------------------------------------------------------------------\n");
    return 0;
}

void TEST_HANDLE_CREATION(void)
{
    const uint16_t BUFFER_SIZE = 32;
    volatile uint8_t buffer[BUFFER_SIZE];
    volatile CBUFF_Struct bufferStruct;
    

    TEST_ASSERT_NOT_EQUAL((CBUFF_Handle)NULL, CBUFF_construct(&bufferStruct, buffer, BUFFER_SIZE));

    TEST_ASSERT_EQUAL((CBUFF_Handle)NULL, CBUFF_construct(NULL, buffer, BUFFER_SIZE));
    TEST_ASSERT_EQUAL((CBUFF_Handle)NULL, CBUFF_construct(&bufferStruct, NULL, BUFFER_SIZE));
    TEST_ASSERT_EQUAL((CBUFF_Handle)NULL, CBUFF_construct(&bufferStruct, buffer, 0));
    TEST_ASSERT_EQUAL((CBUFF_Handle)NULL, CBUFF_construct(NULL, NULL, BUFFER_SIZE));

    TEST_ASSERT_EQUAL(BUFFER_SIZE, CBUFF_construct(&bufferStruct, buffer, BUFFER_SIZE)->capacity);
    TEST_ASSERT_EQUAL(buffer, CBUFF_construct(&bufferStruct, buffer, BUFFER_SIZE)->data);

    TEST_ASSERT_EQUAL(0, CBUFF_construct(&bufferStruct, buffer, BUFFER_SIZE)->count);
    TEST_ASSERT_EQUAL(0, CBUFF_construct(&bufferStruct, buffer, BUFFER_SIZE)->head);
    TEST_ASSERT_EQUAL(0, CBUFF_construct(&bufferStruct, buffer, BUFFER_SIZE)->tail);
}

void TEST_ADD_DATA(void)
{
    const uint16_t BUFFER_SIZE = 32;
    volatile uint8_t buffer[BUFFER_SIZE];
    volatile CBUFF_Struct bufferStruct;
    volatile CBUFF_Handle handle = CBUFF_construct(&bufferStruct, buffer, BUFFER_SIZE);

    uint8_t test_data1 = 42;
    uint8_t test_data2 = 100;

    CBUFF_put(handle, test_data1);
    TEST_ASSERT_EQUAL(1, handle->count);
    TEST_ASSERT_EQUAL(test_data1, handle->data[0]);
    TEST_ASSERT_EQUAL(1, handle->head);
    TEST_ASSERT_EQUAL(0, handle->tail);

    CBUFF_put(handle, test_data2);
    TEST_ASSERT_EQUAL(2, handle->count);
    TEST_ASSERT_EQUAL(test_data1, handle->data[0]);
    TEST_ASSERT_EQUAL(test_data2, handle->data[1]);
    TEST_ASSERT_EQUAL(2, handle->head);
    TEST_ASSERT_EQUAL(0, handle->tail);
}

void TEST_REMOVE_DATA(void)
{
    const uint16_t BUFFER_SIZE = 32;
    volatile uint8_t buffer[BUFFER_SIZE];
    volatile CBUFF_Struct bufferStruct;
    volatile CBUFF_Handle handle = CBUFF_construct(&bufferStruct, buffer, BUFFER_SIZE);

    uint8_t test_data1 = 42;
    uint8_t test_data2 = 100;

    CBUFF_put(handle, test_data1);
    CBUFF_put(handle, test_data2);

    TEST_ASSERT_EQUAL(test_data1, CBUFF_get(handle));
    TEST_ASSERT_EQUAL(1, handle->count);
    TEST_ASSERT_EQUAL(1, handle->tail);
    TEST_ASSERT_EQUAL(2, handle->head);

    TEST_ASSERT_EQUAL(test_data2, CBUFF_get(handle));
    TEST_ASSERT_EQUAL(0, handle->count);
    TEST_ASSERT_EQUAL(2, handle->tail);
    TEST_ASSERT_EQUAL(2, handle->head);
}

void TEST_ADD_DATA_BUFFER_FULL(void)
{
    const uint16_t BUFFER_SIZE = 256;
    volatile uint8_t buffer[BUFFER_SIZE];
    volatile CBUFF_Struct bufferStruct;
    volatile CBUFF_Handle handle = CBUFF_construct(&bufferStruct, buffer, BUFFER_SIZE);

    for(uint16_t i = 0; i < BUFFER_SIZE; i++)
    {
        CBUFF_put(handle, (uint8_t)i);
    }

    TEST_ASSERT_EQUAL(BUFFER_SIZE, handle->count);
    TEST_ASSERT_EQUAL(1, CBUFF_isFull(handle));
    TEST_ASSERT_EQUAL(0, CBUFF_isEmpty(handle));
}

void TEST_REMOVE_DATA_BUFFER_EMPTY(void)
{
    const uint16_t BUFFER_SIZE = 256;
    volatile uint8_t buffer[BUFFER_SIZE];
    volatile CBUFF_Struct bufferStruct;
    volatile CBUFF_Handle handle = CBUFF_construct(&bufferStruct, buffer, BUFFER_SIZE);

    for(uint16_t i = 0; i < BUFFER_SIZE; i++)
    {
        CBUFF_put(handle, (uint8_t)i);
    }

    for(uint16_t i = 0; i < BUFFER_SIZE; i++)
    {
        CBUFF_get(handle);
    }

    TEST_ASSERT_EQUAL(0, handle->count);
    TEST_ASSERT_EQUAL(0, CBUFF_isFull(handle));
    TEST_ASSERT_EQUAL(1, CBUFF_isEmpty(handle));
}

void TEST_ADD_DATA_OVERFLOW(void)
{
    const uint16_t BUFFER_SIZE = 8;
    volatile uint8_t buffer[BUFFER_SIZE];
    volatile CBUFF_Struct bufferStruct;
    volatile CBUFF_Handle handle = CBUFF_construct(&bufferStruct, buffer, BUFFER_SIZE);

    for(uint16_t i = 0; i < BUFFER_SIZE; i++)
    {
        CBUFF_put(handle, (uint8_t)i);
    }

    // put one more in the buffer
    CBUFF_put(handle, 10);

    TEST_ASSERT_EQUAL(BUFFER_SIZE, handle->count);
    TEST_ASSERT_EQUAL(0, handle->head);
    TEST_ASSERT_EQUAL(BUFFER_SIZE - 1, handle->data[BUFFER_SIZE - 1]);

    // put another one more in the buffer
    CBUFF_put(handle, 10);
    TEST_ASSERT_EQUAL(BUFFER_SIZE, handle->count);
    TEST_ASSERT_EQUAL(0, handle->head);
    TEST_ASSERT_EQUAL(BUFFER_SIZE - 1, handle->data[BUFFER_SIZE - 1]);
}

void TEST_REMOVE_DATA_UNDERFLOW(void)
{
    const uint16_t BUFFER_SIZE = 8;
    volatile uint8_t buffer[BUFFER_SIZE];
    volatile CBUFF_Struct bufferStruct;
    volatile CBUFF_Handle handle = CBUFF_construct(&bufferStruct, buffer, BUFFER_SIZE);

    for(uint16_t i = 0; i < BUFFER_SIZE; i++)
    {
        CBUFF_put(handle, (uint8_t)(i + 1));
    }

    for(uint16_t i = 0; i < BUFFER_SIZE; i++)
    {
        CBUFF_get(handle);
    }

    TEST_ASSERT_EQUAL(0, handle->count);
    TEST_ASSERT_EQUAL(0, handle->tail);

    uint8_t tmpData;

    // get one more than already put in
    tmpData = CBUFF_get(handle);
    TEST_ASSERT_EQUAL(0, handle->count);
    TEST_ASSERT_EQUAL(0, handle->tail);
    TEST_ASSERT_EQUAL(0, tmpData);

    // get another one more than already put in
    tmpData = CBUFF_get(handle);
    TEST_ASSERT_EQUAL(0, handle->count);
    TEST_ASSERT_EQUAL(0, handle->tail);
    TEST_ASSERT_EQUAL(0, tmpData);
}

void TEST_PEEK(void)
{
    const uint16_t BUFFER_SIZE = 16;
    volatile uint8_t buffer[BUFFER_SIZE];
    volatile CBUFF_Struct bufferStruct;
    volatile CBUFF_Handle handle = CBUFF_construct(&bufferStruct, buffer, BUFFER_SIZE);

    /* add 3 test byte */
    CBUFF_put(handle, 12);
    CBUFF_put(handle, 20);
    CBUFF_put(handle, 25);

    TEST_ASSERT_EQUAL(12, CBUFF_peek(handle));  /* should return the first available data   */
    TEST_ASSERT_EQUAL(3, handle->count);
    TEST_ASSERT_EQUAL(3, handle->head);
    TEST_ASSERT_EQUAL(0, handle->tail);

    /* add another byte */
    CBUFF_put(handle, 100);
    TEST_ASSERT_EQUAL(12, CBUFF_peek(handle));  /* should return the first available data   */
    TEST_ASSERT_EQUAL(4, handle->count);
    TEST_ASSERT_EQUAL(4, handle->head);
    TEST_ASSERT_EQUAL(0, handle->tail);

    /* remove one byte */
    CBUFF_get(handle);
    TEST_ASSERT_EQUAL(20, CBUFF_peek(handle));  /* should return the first available data   */
}

void TEST_PEEK_UNDERFLOW(void)
{
    const uint16_t BUFFER_SIZE = 8;
    volatile uint8_t buffer[BUFFER_SIZE];
    volatile CBUFF_Struct bufferStruct;
    volatile CBUFF_Handle handle = CBUFF_construct(&bufferStruct, buffer, BUFFER_SIZE);

    CBUFF_put(handle, 10);
    CBUFF_get(handle);

    TEST_ASSERT_EQUAL(0, CBUFF_peek(handle));
}

void TEST_GET_FREE_BYTE(void)
{
    const uint16_t BUFFER_SIZE = 4;
    volatile uint8_t buffer[BUFFER_SIZE];
    volatile CBUFF_Struct bufferStruct;
    volatile CBUFF_Handle handle = CBUFF_construct(&bufferStruct, buffer, BUFFER_SIZE);

    CBUFF_put(handle, 10);
    TEST_ASSERT_EQUAL(BUFFER_SIZE - 1, CBUFF_getNumOfFreeByte(handle));

    CBUFF_put(handle, 20);
    TEST_ASSERT_EQUAL(BUFFER_SIZE - 2, CBUFF_getNumOfFreeByte(handle));

    CBUFF_put(handle, 30);
    TEST_ASSERT_EQUAL(BUFFER_SIZE - 3, CBUFF_getNumOfFreeByte(handle));

    CBUFF_get(handle);
    TEST_ASSERT_EQUAL(BUFFER_SIZE - 2, CBUFF_getNumOfFreeByte(handle));

    CBUFF_get(handle);
    TEST_ASSERT_EQUAL(BUFFER_SIZE - 1, CBUFF_getNumOfFreeByte(handle));

    CBUFF_get(handle);
    TEST_ASSERT_EQUAL(BUFFER_SIZE, CBUFF_getNumOfFreeByte(handle));
}

void TEST_RANDOM_DATA(void)
{
    const uint16_t BUFFER_SIZE = 2048;
    volatile uint8_t buffer[BUFFER_SIZE];
    volatile CBUFF_Struct bufferStruct;
    volatile CBUFF_Handle handle = CBUFF_construct(&bufferStruct, buffer, BUFFER_SIZE);

    /* make some random data */
    srand(time(NULL));
    uint8_t tmpData[256] = {0};
    for(uint16_t i = 0; i < sizeof(tmpData); i++)
    {
        tmpData[i] = rand() % 0xFF;
    }

    /* put data into the buffer */
    for(uint16_t i = 0; i < sizeof(tmpData); i++)
    {
        CBUFF_put(handle, tmpData[i]);
    }

    /* get data out */
    for(uint16_t i = 0; i < sizeof(tmpData); i++)
    {
        TEST_ASSERT_EQUAL(tmpData[i], CBUFF_get(handle));
    }
    
}

void TEST_OBJ_HANDLE_CREATION(void)
{
    const int CAPACITY = 4;
    const int OBJECT_SIZE = sizeof(testObjType);
    volatile uint8_t buffer[CAPACITY * OBJECT_SIZE];
    CBUFF_OBJ_Struct buffStruct;
    CBUFF_OBJ_Handle handle = CBUFF_OBJ_construct(&buffStruct, buffer, OBJECT_SIZE, CAPACITY);

    TEST_ASSERT_NOT_EQUAL((CBUFF_OBJ_Handle)NULL, handle);

    TEST_ASSERT_EQUAL((CBUFF_OBJ_Handle)NULL, CBUFF_OBJ_construct(NULL, buffer, OBJECT_SIZE, CAPACITY));
    TEST_ASSERT_EQUAL((CBUFF_OBJ_Handle)NULL, CBUFF_OBJ_construct(&buffStruct, NULL, OBJECT_SIZE, CAPACITY));
    TEST_ASSERT_EQUAL((CBUFF_OBJ_Handle)NULL, CBUFF_OBJ_construct(&buffStruct, buffer, 0, CAPACITY));
    TEST_ASSERT_EQUAL((CBUFF_OBJ_Handle)NULL, CBUFF_OBJ_construct(&buffStruct, buffer, OBJECT_SIZE, 0));
}

void setUp(void)
{
    
}

void tearDown(void)
{
    
}