#ifndef UARRAY2_INCLUDED
#define UARRAY2_INCLUDED
#define A UArray2_T
#include <uarray.h>

typedef struct A *A;

struct A {
    int width;
    int height;
    int size;
    UArray_T elems;
};

A UArray2_new(int width, int height, int size);

void UArray2_free(A *uarray2);
int UArray2_height(A uarray2);
int UArray2_width(A uarray2);
int UArray2_size(A uarray2);
void * UArray2_at(A uarray2, int col, int row);
void UArray2_map_row_major(A t, void apply(int col, int row, A a,  void *cl), void *cl);
void UArray2_map_col_major(A t, void apply(int col, int row, void *cl), void *cl);

#undef A
#endif