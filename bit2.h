#ifndef BIT2_INCLUDED
#define BIT2_INCLUDED
#define T Bit2_T

typedef struct T *T;

T Bit2_new(int width, int height);
void Bit2_free(T* bit2);

int Bit2_height(T bit2);
int Bit2_width(T bit2);

int Bit2_get(T bit2, int col, int row);
int Bit2_put(T bit2, int col, int row, int n);

void Bit2_map_row_major(T bit2, void apply(int col, int row, T bit2, int b,
        void *cl), void *cl);
void Bit2_map_col_major(T bit2, void apply(int col, int row, T bit2, int b,
        void *cl), void *cl);

#undef T
#endif