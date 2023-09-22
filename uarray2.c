#include "uarray2.h"
#include <uarray.h>


struct T {
    int width;
    int height;
    int size;
    UArray_T * elems;
};

T UArray2_new(T t, int width, int height, int size) {

}

void UArray2_free(T t) {

}

int UArray2_length(T t) {

}

int UArray2_width(T t) {

}

int UArray2_size(T t) {

}

void * UArray2_at(T t, int col, int row) {

}

void UArray2_map_row_major(T t, void apply(int col, int row, T a,  void *cl), 
        void *cl) {

}


/* Description: Applies a function to each element of the UArray2 by going across rows 
        first, hitting every element in the column and across by column.

   Input: t - UArray2 type
        An apply function, to perform on each element, whose parameters are:
                a 2d index: (col, row), t – UArray2 type, a void pointer to the current element, and a closure variable to pass information between each call.
	the closure variable - used to pass extra information between each iteration.

   Expectations:
	the UArray2 passed in is not null
        the apply function is not null
        the address of the UArray2 passed is the same as the instance of the UArray2.
*/
void UArray2_map_col_major(T t, void apply(int col, int row, void *cl), 
        void *cl) {

}