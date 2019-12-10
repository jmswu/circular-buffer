#include <stdio.h>
#include "cbuff.h"

int main(char *argv[], int argc){

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

    //printf("hello world!\n");
    return 0;
}