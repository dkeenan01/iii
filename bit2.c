#include <bit.h>
#include <bit2.h>
#include <mem.h>
#include <assert.h>

#define T Bit2_T


struct T {
    int width;
    int height;
    Bit_T elems;
};

T Bit2_new(int width, int height){
    assert(width >= 0 && height >= 0);
    T bit2;
    NEW(bit2);
    bit2 -> width = width;
    bit2 -> height = height;
    bit2 -> elems = Bit_new(width * height);
    return bit2;
}
void Bit2_free(T *bit2){
    assert(bit2 && *bit2);
    Bit_free(&((*bit2)->elems));
    FREE(*bit2);
}

int Bit2_height(T bit2){
    assert(bit2);
    return bit2->height;
}
int Bit2_width(T bit2){
    assert(bit2);
    return bit2->width;
}

int Bit2_flattened_index(int col, int row, int width){
    return width * row + col;
}

int Bit2_get(T bit2, int col, int row){
    assert(bit2);

    assert(col < bit2->width && col >= 0);
    assert(row < bit2->height && row >= 0);

    int flat_index = Bit2_flattened_index(col, row, bit2->width);

    return Bit_get(bit2->elems, flat_index);
}

int Bit2_put(T bit2, int col, int row, int n){
    assert(bit2);

    assert(col < bit2->width && col >= 0);
    assert(row < bit2->height && row >= 0);

    int flat_index = Bit2_flattened_index(col, row, bit2->width);

    return Bit_put(bit2->elems, flat_index, n);
}

void Bit2_map_row_major(T bit2, 
                        void apply(int col, int row, T bit2, int b, void *cl),
                        void *cl){
    assert(bit2 && apply);

    for (int row = 0; row < bit2->height; row++) {
            for (int col = 0; col < bit2->width; col++) {
                    apply(col, row, bit2, 
                            Bit2_get(bit2, col, row), cl);
            }
    }
}
void Bit2_map_col_major(T bit2,
                        void apply(int col, int row, T bit2, int b, void *cl),
                        void *cl){
    assert(bit2 && apply);

    for (int col = 0; col < bit2->width; col++) {
            for (int row = 0; row < bit2->height; row++) {
                    apply(col, row, bit2, 
                            Bit2_get(bit2, col, row), cl);
            }
    }
} 



#undef T