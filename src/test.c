#include <stdio.h>
#include "cbuff.h"
#include "Unity/unity.h"

void TEST_HANDLE_CREATION(void);
void TEST_ADD_DATA(void);

int main(int argc, char ** argv){

    /* dummp code to get rid of warnings */
    argc++;
    argv++;

    TEST_HANDLE_CREATION();
    TEST_ADD_DATA();

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

void setUp(void)
{

}

void tearDown(void)
{
    
}