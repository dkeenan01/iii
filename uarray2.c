#include "uarray2.h"
#include <uarray.h>
#include <mem.h>
#include <assert.h>
#define T UArray2_T

struct T {
    int width;
    int height;
    int size;
    UArray_T elems;
};

T UArray2_new(int width, int height, int size) {
        A array;
        NEW(array);
        array->width = width;
        array->height = height;
        array->size = size;
        array->elems = UArray_new(width * height, size);
        return array;
}

void UArray2_free(T *uarray2) {
        assert(uarray2 && *uarray2);
        UArray_free(&((*uarray2)->elems));
        FREE(*uarray2);
}

int UArray2_height(T uarray2) {
        assert(uarray2);
        return uarray2->height;
}

int UArray2_width(T uarray2) {
        assert(uarray2);
        return uarray2->width;
}

int UArray2_size(T uarray2) {
        assert(uarray2);
        return uarray2->size;
}

int UArray2_flattened_index(int col, int row, int width){
        return width * row + col;
}

void * UArray2_at(T uarray2, int col, int row){
        assert(uarray2);

        assert(col < uarray2->width && col >= 0);
        assert(row < uarray2->height && row >= 0);

        int flat_index = UArray2_flattened_index(col, row, uarray2->width);

        return UArray_at(uarray2->elems, flat_index);
}

// void UArray2_map_row_major(T uarray2, void apply(int col, int row, T uarray2, 
//      void* elem, void *cl), void *cl) {

// }


// /* Description: 
//         Applies a function to each element of the UArray2 by going across rows 
//         first, hitting every element in the column and across by column.
//    Input: 
//         t - UArray2 type
//         An apply function, to perform on each element, whose parameters are:
//                 a 2d index: (col, row), t – UArray2 type, a void pointer to 
//                 the current element, and a closure variable to pass 
//                 information between each call.
// 	the closure variable - used to pass extra information between each 
//                 iteration.
//    Expectations:
// 	the UArray2 passed in is not null
//         the apply function is not null
//         the address of the UArray2 passed is the same as the instance 
//         of the UArray2.                                                         */
// void UArray2_map_col_major(T uarray2, void apply(int col, int row, T uarray2,
//      void* elem, void *cl), void *cl) {

// }

#undef T