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

void UArray2_map_row_major(T uarray2, void apply(int col, int row, T uarray2, 
     void* elem, void *cl), void *cl) {

        assert(uarray2 && apply);

        for (int row = 0; row < uarray2->height; row++) {
                for (int col = 0; col < uarray2->width; col++) {
                        apply(col, row, uarray2, 
                                UArray2_at(uarray2, col, row), cl);
                }
        }
}

void UArray2_map_col_major(T uarray2, void apply(int col, int row, T uarray2,
     void* elem, void *cl), void *cl) {

        assert(uarray2 && apply);

        for (int col = 0; col < uarray2->width; col++) {
                for (int row = 0; row < uarray2->height; row++) {
                        apply(col, row, uarray2, 
                                UArray2_at(uarray2, col, row), cl);
                }
        }
}

#undef T