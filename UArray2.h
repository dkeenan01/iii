#ifndef UARRAY2_INCLUDED
#define UARRAY2_INCLUDED
#define T UArray2_T
typedef struct T *T;

T UArray2_new(T t, int width, int height, int size);

void UArray2_free(T t);
int UArray2_length(T t);
int UArray2_width(T t);
int UArray2_size(T t);
void * UArray2_at(T t, int x, int y);
void UArray2_map_row_major(T t, void apply(int col, int row, T a,  void *cl), void *cl);
void UArray2_map_col_major(T t, void apply(int col, int row, void *cl), void *cl);
#undef T
#endif