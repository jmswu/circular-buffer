#include <stdio.h>
#include "cbuff.h"
#include "Unity/unity.h"

void TEST_HANDLE_CREATION(void);
void TEST_ADD_DATA(void);
void TEST_ADD_DATA_BUFFER_FULL(void);
void TEST_ADD_DATA_OVERFLOW(void);
void TEST_REMOVE_DATA(void);
void TEST_REMOVE_DATA_BUFFER_EMPTY(void);
void TEST_REMOVE_DATA_UNDERFLOW(void);

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

    printf("done!\n\r");

    return 0;
}

void TEST_HANDLE_CREATION(void)
{
    const uint16_t BUFFER_SIZE = 30;
    volatile uint8_t buffer[BUFFER_SIZE];
    volatile CBUFF_Struct bufferStruct;
    

    TEST_ASSERT_NOT_EQUAL((CBUFF_Handle)NULL, CBUFF_construct(&bufferStruct, buffer, BUFFER_SIZE));

    TEST_ASSERT_EQUAL((CBUFF_Handle)NULL, CBUFF_construct(NULL, buffer, BUFFER_SIZE));
    TEST_ASSERT_EQUAL((CBUFF_Handle)NULL, CBUFF_construct(&bufferStruct, NULL, BUFFER_SIZE));
    TEST_ASSERT_EQUAL((CBUFF_Handle)NULL, CBUFF_construct(NULL, NULL, BUFFER_SIZE));

    TEST_ASSERT_EQUAL(BUFFER_SIZE, CBUFF_construct(&bufferStruct, buffer, BUFFER_SIZE)->capacity);
    TEST_ASSERT_EQUAL(buffer, CBUFF_construct(&bufferStruct, buffer, BUFFER_SIZE)->data);

    TEST_ASSERT_EQUAL(0, CBUFF_construct(&bufferStruct, buffer, BUFFER_SIZE)->count);
    TEST_ASSERT_EQUAL(0, CBUFF_construct(&bufferStruct, buffer, BUFFER_SIZE)->head);
    TEST_ASSERT_EQUAL(0, CBUFF_construct(&bufferStruct, buffer, BUFFER_SIZE)->tail);
}

void TEST_ADD_DATA(void)
{
    const uint16_t BUFFER_SIZE = 30;
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
    const uint16_t BUFFER_SIZE = 30;
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
    const uint16_t BUFFER_SIZE = 0xFFFF;
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
    const uint16_t BUFFER_SIZE = 0xFFFF;
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
    const uint16_t BUFFER_SIZE = 5;
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
    const uint16_t BUFFER_SIZE = 5;
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

void setUp(void)
{

}

void tearDown(void)
{
    
}