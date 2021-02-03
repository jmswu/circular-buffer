#include <stdio.h>
#include "cbuff.h"
#include "Unity/unity.h"

void TEST_HANDLE_CREATION(void);

int main(int argc, char ** argv){

    /* dummp code to get rid of warnings */
    argc++;
    argv++;

    TEST_HANDLE_CREATION();

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
}

void setUp(void)
{

}

void tearDown(void)
{
    
}