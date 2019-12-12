#include <stdio.h>
#include "cbuff.h"

typedef struct{
    uint8_t data1;
    uint8_t data2;
    uint16_t data3;
}test_t;

int main(char *argv[], int argc){

    printf("Testing circular buffer!\n");
    volatile CBUFF_Struct cbuff;
    volatile uint8_t data[10];

    CBUFF_Handle handle = CBUFF_construct(&cbuff, data, sizeof(data)/sizeof(data[0]));

    if (!CBUFF_isFull(handle)) CBUFF_put(handle, 'H');
    if (!CBUFF_isFull(handle)) CBUFF_put(handle, 'E');
    if (!CBUFF_isFull(handle)) CBUFF_put(handle, 'L');
    if (!CBUFF_isFull(handle)) CBUFF_put(handle, 'L');
    if (!CBUFF_isFull(handle)) CBUFF_put(handle, 'O');
    if (!CBUFF_isFull(handle)) CBUFF_put(handle, 'W');

    while (!CBUFF_isEmpty(handle)){
        printf("%c", CBUFF_get(handle));
    }

    printf("Testing circular object buffer!\n");
    volatile CBUFF_OBJ_Struct cbuff_obj;
    volatile uint8_t data_obj[10 * sizeof(test_t)];

    CBUFF_OBJ_Handle handle_obj = CBUFF_OBJ_construct(&cbuff_obj, data_obj, sizeof(test_t), sizeof(data_obj)/sizeof(test_t));

    test_t test;
    for(int i = 0; i < 20; i++){
        test.data1 = i * 1;
        test.data2 = i * 10;
        test.data3 = i * 100;
        if (!CBUFF_OBJ_isFull(handle_obj)) CBUFF_OBJ_put(handle_obj, &test);
        // printf("iteration: %d, isEmpty: %d\n", i, CBUFF_OBJ_isEmpty(handle_obj));
        // printf("iteration: %d, isFull : %d\n", i, CBUFF_OBJ_isFull(handle_obj));
        // printf("test: %d, %d, %d\n", test.data1, test.data2, test.data3);
    }

    for(int i = 0; i < 20; i++){
        if (!CBUFF_OBJ_isEmpty(handle_obj)){
            CBUFF_OBJ_get(handle_obj, &test);
            printf("test: %d, %d, %d\n", test.data1, test.data2, test.data3);
        }
        printf("iteration: %d, isEmpty: %d\n", i, CBUFF_OBJ_isEmpty(handle_obj));
        printf("iteration: %d, isFull : %d\n", i, CBUFF_OBJ_isFull(handle_obj));
        printf("\n");
    }
    
    return 0;
}