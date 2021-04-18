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
void TEST_IS_EMPTY(void);
void TEST_IS_FULL(void);
void TEST_OVERFLOWCOUNT(void);
void TEST_UNDERFLOWCOUNT(void);

void TEST_OBJ_HANDLE_CREATION(void);
void TEST_OBJ_ADD_DATA(void);
void TEST_OBJ_ADD_DATA_BUFFER_FULL(void);
void TEST_OBJ_ADD_DATA_OVERFLOW(void);
void TEST_OBJ_REMOVE_DATA(void);
void TEST_OBJ_REMOVE_DATA_BUFFER_EMPTY(void);
void TEST_OBJ_REMOVE_DATA_UNDERFLOW(void);
void TEST_OBJ_PEEK(void);
void TEST_OBJ_PEEK_UNDERFLOW(void);
void TEST_OBJ_GET_FREE_CAPACITY(void);
void TEST_OBJ_RANDOM_DATA(void);
void TEST_OBJ_IS_EMPTY(void);
void TEST_OBJ_IS_FULL(void);
void TEST_OBJ_OVERFLOW(void);
void TEST_OBJ_UNDERFLOW(void);

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

    srand(time(NULL));

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
    TEST_IS_EMPTY();
    TEST_IS_FULL();
    TEST_OVERFLOWCOUNT();
    TEST_UNDERFLOWCOUNT();

    TEST_OBJ_HANDLE_CREATION();
    TEST_OBJ_ADD_DATA();
    TEST_OBJ_ADD_DATA_BUFFER_FULL();
    TEST_OBJ_ADD_DATA_OVERFLOW();
    TEST_OBJ_REMOVE_DATA();
    TEST_OBJ_REMOVE_DATA_BUFFER_EMPTY();
    TEST_OBJ_REMOVE_DATA_UNDERFLOW();
    TEST_OBJ_PEEK();
    TEST_OBJ_PEEK_UNDERFLOW();
    TEST_OBJ_GET_FREE_CAPACITY();
    TEST_OBJ_RANDOM_DATA();
    TEST_OBJ_IS_EMPTY();
    TEST_OBJ_IS_FULL();
    TEST_OBJ_OVERFLOW();
    TEST_OBJ_UNDERFLOW();

    /* CBUFF_OBJ is not tested */

    return printEndingMessage();
}

int printEndingMessage(void)
{
    printf("-------------------------------------------------------------------------\n");
    printf("Testin circular buffer library, v%u.%u.%u\n",
        (CBUFF_getVerNumber() & 0x00FF0000) >> 16,
        (CBUFF_getVerNumber() & 0x0000FF00) >> 8,
        (CBUFF_getVerNumber() & 0x000000FF));
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

void TEST_IS_EMPTY(void)
{
    const uint16_t BUFFER_SIZE = 2048;
    volatile uint8_t buffer[BUFFER_SIZE];
    volatile CBUFF_Struct bufferStruct;
    volatile CBUFF_Handle handle = CBUFF_construct(&bufferStruct, buffer, BUFFER_SIZE);

    TEST_ASSERT_EQUAL(1, CBUFF_isEmpty(handle));
    TEST_ASSERT_EQUAL(1, CBUFF_isEmpty(NULL));

    TEST_ASSERT_EQUAL(0, CBUFF_isDataAvailable(handle));
    TEST_ASSERT_EQUAL(0, CBUFF_isDataAvailable(NULL));

    CBUFF_put(handle, 1);
    TEST_ASSERT_EQUAL(0, CBUFF_isEmpty(handle));
    TEST_ASSERT_EQUAL(1, CBUFF_isEmpty(NULL));

    TEST_ASSERT_EQUAL(1, CBUFF_isDataAvailable(handle));
    TEST_ASSERT_EQUAL(0, CBUFF_isDataAvailable(NULL));

    CBUFF_get(handle);
    TEST_ASSERT_EQUAL(1, CBUFF_isEmpty(handle));
    TEST_ASSERT_EQUAL(1, CBUFF_isEmpty(NULL));

    TEST_ASSERT_EQUAL(0, CBUFF_isDataAvailable(handle));
    TEST_ASSERT_EQUAL(0, CBUFF_isDataAvailable(NULL));
}

void TEST_IS_FULL(void)
{
    const uint16_t BUFFER_SIZE = 4;
    volatile uint8_t buffer[BUFFER_SIZE];
    volatile CBUFF_Struct bufferStruct;
    volatile CBUFF_Handle handle = CBUFF_construct(&bufferStruct, buffer, BUFFER_SIZE);

    TEST_ASSERT_EQUAL(0, CBUFF_isFull(handle));
    TEST_ASSERT_EQUAL(1, CBUFF_isFull(NULL));

    TEST_ASSERT_EQUAL(1, CBUFF_isEmptySpaceAvailable(handle));
    TEST_ASSERT_EQUAL(0, CBUFF_isEmptySpaceAvailable(NULL));

    for(uint16_t i = 0; i < BUFFER_SIZE; i++)
    {
        CBUFF_put(handle, 1);
    }

    TEST_ASSERT_EQUAL(1, CBUFF_isFull(handle));
    TEST_ASSERT_EQUAL(1, CBUFF_isFull(NULL));

    TEST_ASSERT_EQUAL(0, CBUFF_isEmptySpaceAvailable(handle));
    TEST_ASSERT_EQUAL(0, CBUFF_isEmptySpaceAvailable(NULL));

    CBUFF_get(handle);
    TEST_ASSERT_EQUAL(0, CBUFF_isFull(handle));
    TEST_ASSERT_EQUAL(1, CBUFF_isFull(NULL));

    TEST_ASSERT_EQUAL(1, CBUFF_isEmptySpaceAvailable(handle));
    TEST_ASSERT_EQUAL(0, CBUFF_isEmptySpaceAvailable(NULL));
}

void TEST_OVERFLOWCOUNT(void)
{
    const uint16_t BUFFER_SIZE = 4;
    volatile uint8_t buffer[BUFFER_SIZE];
    volatile CBUFF_Struct bufferStruct;
    volatile CBUFF_Handle handle = CBUFF_construct(&bufferStruct, buffer, BUFFER_SIZE);

    TEST_ASSERT_EQUAL(0, handle->overFlowCount);
    TEST_ASSERT_EQUAL(0, CBUFF_getOverflowCounts(handle));

    /* fill up the buffer */
    for(uint16_t i = 0; i < BUFFER_SIZE; i++)
    {
        CBUFF_put(handle, 1);
    }

    CBUFF_put(handle, 0xFF);
    TEST_ASSERT_EQUAL(1, handle->overFlowCount);
    TEST_ASSERT_EQUAL(1, CBUFF_getOverflowCounts(handle));

    CBUFF_put(handle, 0xFF);
    TEST_ASSERT_EQUAL(2, handle->overFlowCount);
    TEST_ASSERT_EQUAL(2, CBUFF_getOverflowCounts(handle));

    TEST_ASSERT_EQUAL_UINT(~0, CBUFF_getUnderflowCounts(NULL));
}

void TEST_UNDERFLOWCOUNT(void) 
{
    const uint16_t BUFFER_SIZE = 4;
    volatile uint8_t buffer[BUFFER_SIZE];
    volatile CBUFF_Struct bufferStruct;
    volatile CBUFF_Handle handle = CBUFF_construct(&bufferStruct, buffer, BUFFER_SIZE);

    TEST_ASSERT_EQUAL(0, handle->underFlowCount);
    TEST_ASSERT_EQUAL(0, CBUFF_getUnderflowCounts(handle));

    /* fill up the buffer */
    for(uint16_t i = 0; i < BUFFER_SIZE; i++)
    {
        CBUFF_put(handle, 1);
    }

    TEST_ASSERT_EQUAL(0, handle->underFlowCount);
    TEST_ASSERT_EQUAL(0, CBUFF_getUnderflowCounts(handle));

    /* empty the buffer */
    for(uint16_t i = 0; i < BUFFER_SIZE; i++)
    {
        CBUFF_get(handle);
    }

    CBUFF_get(handle);
    TEST_ASSERT_EQUAL(1, handle->underFlowCount);
    TEST_ASSERT_EQUAL(1, CBUFF_getUnderflowCounts(handle));

    CBUFF_get(handle);
    TEST_ASSERT_EQUAL(2, handle->underFlowCount);
    TEST_ASSERT_EQUAL(2, CBUFF_getUnderflowCounts(handle));

    TEST_ASSERT_EQUAL_UINT(~0, CBUFF_getUnderflowCounts(NULL));

}

void TEST_OBJ_HANDLE_CREATION(void)
{
    const int CAPACITY = 4;
    const int OBJECT_SIZE = sizeof(testObjType);
    volatile uint8_t buffer[CAPACITY * OBJECT_SIZE];
    CBUFF_OBJ_Struct buffStruct;
    CBUFF_OBJ_Handle handle = CBUFF_OBJ_construct(&buffStruct, buffer, OBJECT_SIZE, CAPACITY);

    TEST_ASSERT_NOT_EQUAL((CBUFF_OBJ_Handle)NULL, handle);
    TEST_ASSERT_EQUAL(CAPACITY, handle->capacity);
    TEST_ASSERT_EQUAL(OBJECT_SIZE, handle->obj_size);
    TEST_ASSERT_EQUAL(0, handle->count);
    TEST_ASSERT_EQUAL(0, handle->head);
    TEST_ASSERT_EQUAL(0, handle->tail);

    TEST_ASSERT_EQUAL((CBUFF_OBJ_Handle)NULL, CBUFF_OBJ_construct(NULL, buffer, OBJECT_SIZE, CAPACITY));
    TEST_ASSERT_EQUAL((CBUFF_OBJ_Handle)NULL, CBUFF_OBJ_construct(&buffStruct, NULL, OBJECT_SIZE, CAPACITY));
    TEST_ASSERT_EQUAL((CBUFF_OBJ_Handle)NULL, CBUFF_OBJ_construct(&buffStruct, buffer, 0, CAPACITY));
    TEST_ASSERT_EQUAL((CBUFF_OBJ_Handle)NULL, CBUFF_OBJ_construct(&buffStruct, buffer, OBJECT_SIZE, 0));
}

void TEST_OBJ_ADD_DATA(void)
{
    const int CAPACITY = 4;
    const int OBJECT_SIZE = sizeof(testObjType);
    volatile uint8_t buffer[CAPACITY * OBJECT_SIZE];
    CBUFF_OBJ_Struct buffStruct;
    CBUFF_OBJ_Handle handle = CBUFF_OBJ_construct(&buffStruct, buffer, OBJECT_SIZE, CAPACITY);

    testObjType testObj = 
    {
        .data1 = 255,
        .data2 = 1024,
        .data3 = 0xFFFFF,
    };

    TEST_ASSERT_EQUAL(1, CBUFF_OBJ_put(handle, &testObj));
    TEST_ASSERT_EQUAL(1, handle->count);
    TEST_ASSERT_EQUAL(1, handle->head);
    TEST_ASSERT_EQUAL(0, handle->tail);

    TEST_ASSERT_EQUAL(0, CBUFF_OBJ_put(NULL, &testObj));
    TEST_ASSERT_EQUAL(0, CBUFF_OBJ_put(handle, NULL));
    TEST_ASSERT_EQUAL(0, CBUFF_OBJ_put(NULL, NULL));
}

void TEST_OBJ_ADD_DATA_BUFFER_FULL(void)
{
    const int CAPACITY = 4;
    const int OBJECT_SIZE = sizeof(testObjType);
    volatile uint8_t buffer[CAPACITY * OBJECT_SIZE];
    CBUFF_OBJ_Struct buffStruct;
    CBUFF_OBJ_Handle handle = CBUFF_OBJ_construct(&buffStruct, buffer, OBJECT_SIZE, CAPACITY);

    testObjType testObj = 
    {
        .data1 = 255,
        .data2 = 1024,
        .data3 = 0xFFFFF,
    };

    for(uint16_t i = 0; i < CAPACITY; i++)
    {
        TEST_ASSERT_EQUAL(1, CBUFF_OBJ_put(handle, &testObj));
    }

    TEST_ASSERT_EQUAL(CAPACITY, handle->count);
    TEST_ASSERT_EQUAL(0, handle->head);
    TEST_ASSERT_EQUAL(0, handle->tail);
}

void TEST_OBJ_ADD_DATA_OVERFLOW(void)
{
    const int CAPACITY = 4;
    const int OBJECT_SIZE = sizeof(testObjType);
    volatile uint8_t buffer[CAPACITY * OBJECT_SIZE];
    CBUFF_OBJ_Struct buffStruct;
    CBUFF_OBJ_Handle handle = CBUFF_OBJ_construct(&buffStruct, buffer, OBJECT_SIZE, CAPACITY);

    testObjType testObj = 
    {
        .data1 = 255,
        .data2 = 1024,
        .data3 = 0xFFFFF,
    };

    for(uint16_t i = 0; i < CAPACITY; i++)
    {
        CBUFF_OBJ_put(handle, &testObj);
    }

    TEST_ASSERT_EQUAL(0, CBUFF_OBJ_put(handle, &testObj));
    TEST_ASSERT_EQUAL(CAPACITY, handle->count);
    TEST_ASSERT_EQUAL(0, handle->head);
    TEST_ASSERT_EQUAL(0, handle->tail);
}

void TEST_OBJ_REMOVE_DATA(void)
{
    const int CAPACITY = 4;
    const int OBJECT_SIZE = sizeof(testObjType);
    volatile uint8_t buffer[CAPACITY * OBJECT_SIZE];
    CBUFF_OBJ_Struct buffStruct;
    CBUFF_OBJ_Handle handle = CBUFF_OBJ_construct(&buffStruct, buffer, OBJECT_SIZE, CAPACITY);

    testObjType testObj = 
    {
        .data1 = 255,
        .data2 = 1024,
        .data3 = 0xFFFFF,
    };

    testObjType testObjGet;

    CBUFF_OBJ_put(handle, &testObj);

    TEST_ASSERT_EQUAL(1, CBUFF_OBJ_get(handle, &testObjGet));
    TEST_ASSERT_EQUAL(0, memcmp(&testObj, &testObjGet, OBJECT_SIZE));

    TEST_ASSERT_EQUAL(0, CBUFF_OBJ_get(handle, &testObjGet));
}

void TEST_OBJ_REMOVE_DATA_BUFFER_EMPTY(void)
{
    const int CAPACITY = 4;
    const int OBJECT_SIZE = sizeof(testObjType);
    volatile uint8_t buffer[CAPACITY * OBJECT_SIZE];
    CBUFF_OBJ_Struct buffStruct;
    CBUFF_OBJ_Handle handle = CBUFF_OBJ_construct(&buffStruct, buffer, OBJECT_SIZE, CAPACITY);

    testObjType testObj = 
    {
        .data1 = 255,
        .data2 = 1024,
        .data3 = 0xFFFFF,
    };

    testObjType testObjGet;

    CBUFF_OBJ_put(handle, &testObj);
    CBUFF_OBJ_get(handle, &testObjGet);

    TEST_ASSERT_EQUAL(0, CBUFF_OBJ_get(handle, &testObjGet));
}

void TEST_OBJ_REMOVE_DATA_UNDERFLOW(void)
{
    const int CAPACITY = 4;
    const int OBJECT_SIZE = sizeof(testObjType);
    volatile uint8_t buffer[CAPACITY * OBJECT_SIZE];
    CBUFF_OBJ_Struct buffStruct;
    CBUFF_OBJ_Handle handle = CBUFF_OBJ_construct(&buffStruct, buffer, OBJECT_SIZE, CAPACITY);

    testObjType testObj = 
    {
        .data1 = 255,
        .data2 = 1024,
        .data3 = 0xFFFFF,
    };

    testObjType testObjGet;

    CBUFF_OBJ_put(handle, &testObj);
    CBUFF_OBJ_get(handle, &testObjGet);

    TEST_ASSERT_EQUAL(0, CBUFF_OBJ_get(handle, &testObjGet));
}

void TEST_OBJ_PEEK(void)
{
    const int CAPACITY = 4;
    const int OBJECT_SIZE = sizeof(testObjType);
    volatile uint8_t buffer[CAPACITY * OBJECT_SIZE];
    CBUFF_OBJ_Struct buffStruct;
    CBUFF_OBJ_Handle handle = CBUFF_OBJ_construct(&buffStruct, buffer, OBJECT_SIZE, CAPACITY);

    testObjType testObj1 = 
    {
        .data1 = 1,
        .data2 = 1024,
        .data3 = 0xFFFFF,
    };

    testObjType testObj2 = 
    {
        .data1 = 2,
        .data2 = 1024,
        .data3 = 0xFFFFF,
    };

    testObjType testObjGet1, testObjGet2;

    CBUFF_OBJ_put(handle, &testObj1);
    CBUFF_OBJ_put(handle, &testObj2);

    TEST_ASSERT_EQUAL(1, CBUFF_OBJ_peek(handle, &testObjGet1));
    TEST_ASSERT_EQUAL(0, memcmp(&testObj1, &testObjGet1, OBJECT_SIZE));

    CBUFF_OBJ_get(handle, &testObjGet2);
    TEST_ASSERT_EQUAL(0, memcmp(&testObj1, &testObjGet2, OBJECT_SIZE));
}

void TEST_OBJ_PEEK_UNDERFLOW(void)
{
    const int CAPACITY = 4;
    const int OBJECT_SIZE = sizeof(testObjType);
    volatile uint8_t buffer[CAPACITY * OBJECT_SIZE];
    CBUFF_OBJ_Struct buffStruct;
    CBUFF_OBJ_Handle handle = CBUFF_OBJ_construct(&buffStruct, buffer, OBJECT_SIZE, CAPACITY);

    testObjType testObjGet;
    TEST_ASSERT_EQUAL(0, CBUFF_OBJ_peek(handle, &testObjGet));
}

void TEST_OBJ_GET_FREE_CAPACITY(void)
{
    const int CAPACITY = 4;
    const int OBJECT_SIZE = sizeof(testObjType);
    volatile uint8_t buffer[CAPACITY * OBJECT_SIZE];
    CBUFF_OBJ_Struct buffStruct;
    CBUFF_OBJ_Handle handle = CBUFF_OBJ_construct(&buffStruct, buffer, OBJECT_SIZE, CAPACITY);

    testObjType testObj = 
    {
        .data1 = 1,
        .data2 = 1024,
        .data3 = 0xFFFFF,
    };

    TEST_ASSERT_EQUAL(CAPACITY, CBUFF_OBJ_getNumOfFreeSlot(handle));

    CBUFF_OBJ_put(handle, &testObj);
    TEST_ASSERT_EQUAL(CAPACITY - 1, CBUFF_OBJ_getNumOfFreeSlot(handle));

    CBUFF_OBJ_put(handle, &testObj);
    TEST_ASSERT_EQUAL(CAPACITY - 2, CBUFF_OBJ_getNumOfFreeSlot(handle));

    CBUFF_OBJ_put(handle, &testObj);
    TEST_ASSERT_EQUAL(CAPACITY - 3, CBUFF_OBJ_getNumOfFreeSlot(handle));

    CBUFF_OBJ_put(handle, &testObj);
    TEST_ASSERT_EQUAL(CAPACITY - 4, CBUFF_OBJ_getNumOfFreeSlot(handle));
}

void TEST_OBJ_RANDOM_DATA(void)
{
    const int CAPACITY = 1024;
    const int OBJECT_SIZE = sizeof(testObjType);
    volatile uint8_t buffer[CAPACITY * OBJECT_SIZE];
    CBUFF_OBJ_Struct buffStruct;
    CBUFF_OBJ_Handle handle = CBUFF_OBJ_construct(&buffStruct, buffer, OBJECT_SIZE, CAPACITY);

    const int TEST_SIZE = 256;
    testObjType testObj[TEST_SIZE];

    /* make some random data */
    for (uint16_t i = 0; i < TEST_SIZE; i++)
    {
        testObj[i].data1 = (uint8_t)rand();
        testObj[i].data2 = (uint16_t)rand();
        testObj[i].data3 = (uint32_t)rand();
    }

    /* put data into buffer */
    for (uint16_t i = 0; i < TEST_SIZE; i++)
    {
        CBUFF_OBJ_put(handle, &testObj[i]);
    }

    /* read data out */
    for (uint16_t i = 0; i < TEST_SIZE; i++)
    {
        testObjType out;
        CBUFF_OBJ_get(handle, &out);
        TEST_ASSERT_EQUAL(0, memcmp(&testObj[i], &out, OBJECT_SIZE));
    }
}

void TEST_OBJ_IS_EMPTY(void)
{
    const int CAPACITY = 4;
    const int OBJECT_SIZE = sizeof(testObjType);
    volatile uint8_t buffer[CAPACITY * OBJECT_SIZE];
    CBUFF_OBJ_Struct buffStruct;
    CBUFF_OBJ_Handle handle = CBUFF_OBJ_construct(&buffStruct, buffer, OBJECT_SIZE, CAPACITY);

    TEST_ASSERT_EQUAL(1, CBUFF_OBJ_isEmpty(handle));
    TEST_ASSERT_EQUAL(1, CBUFF_OBJ_isEmpty(NULL));

    testObjType testObj = 
    {
        .data1 = 1,
        .data2 = 1024,
        .data3 = 0xFFFFF,
    };

    CBUFF_OBJ_put(handle, &testObj);
    TEST_ASSERT_EQUAL(0, CBUFF_OBJ_isEmpty(handle));
}

void TEST_OBJ_IS_FULL(void)
{
    const int CAPACITY = 4;
    const int OBJECT_SIZE = sizeof(testObjType);
    volatile uint8_t buffer[CAPACITY * OBJECT_SIZE];
    CBUFF_OBJ_Struct buffStruct;
    CBUFF_OBJ_Handle handle = CBUFF_OBJ_construct(&buffStruct, buffer, OBJECT_SIZE, CAPACITY);

    TEST_ASSERT_EQUAL(0, CBUFF_OBJ_isFull(handle));
    TEST_ASSERT_EQUAL(1, CBUFF_OBJ_isFull(NULL));

    testObjType testObj = 
    {
        .data1 = 1,
        .data2 = 1024,
        .data3 = 0xFFFFF,
    };

    for(uint16_t i = 0; i < CAPACITY; i++)
    {
        CBUFF_OBJ_put(handle, &testObj);            
    }
    TEST_ASSERT_EQUAL(1, CBUFF_OBJ_isFull(handle));
}

void TEST_OBJ_OVERFLOW(void)
{
    const int CAPACITY = 4;
    const int OBJECT_SIZE = sizeof(testObjType);
    volatile uint8_t buffer[CAPACITY * OBJECT_SIZE];
    CBUFF_OBJ_Struct buffStruct;
    CBUFF_OBJ_Handle handle = CBUFF_OBJ_construct(&buffStruct, buffer, OBJECT_SIZE, CAPACITY);

    TEST_ASSERT_NOT_EQUAL(handle, NULL);

    testObjType testObj = 
    {
        .data1 = 1,
        .data2 = 1024,
        .data3 = 0xFFFFF,
    };

    for(int i = 0; i < CAPACITY; i++)
    {
        CBUFF_OBJ_put(handle, &testObj);
        TEST_ASSERT_EQUAL(0, handle->overflowCount);
    }

    for(int i = 0; i < 4096; i++)
    {
        CBUFF_OBJ_put(handle, &testObj);
        TEST_ASSERT_EQUAL(i + 1, handle->overflowCount);
    }
}

void TEST_OBJ_UNDERFLOW(void)
{
    const int CAPACITY = 4;
    const int OBJECT_SIZE = sizeof(testObjType);
    volatile uint8_t buffer[CAPACITY * OBJECT_SIZE];
    CBUFF_OBJ_Struct buffStruct;
    CBUFF_OBJ_Handle handle = CBUFF_OBJ_construct(&buffStruct, buffer, OBJECT_SIZE, CAPACITY);

    TEST_ASSERT_NOT_EQUAL(handle, NULL);

    testObjType testObj = 
    {
        .data1 = 1,
        .data2 = 1024,
        .data3 = 0xFFFFF,
    };

    testObjType testObjGet = {0};

    for(int i = 0; i < CAPACITY; i++)
    {
        CBUFF_OBJ_put(handle, &testObj);
        TEST_ASSERT_EQUAL(0, handle->underflowCount);
    }

    for(int i = 0; i < CAPACITY; i++)
    {
        CBUFF_OBJ_get(handle, &testObjGet);
        TEST_ASSERT_EQUAL(0, handle->underflowCount);
    }

    for(int i = 0; i < 12345; i++)
    {
        CBUFF_OBJ_get(handle, &testObjGet);
        TEST_ASSERT_EQUAL(i + 1, handle->underflowCount);
    }
}

void setUp(void)
{
    
}

void tearDown(void)
{
    
}