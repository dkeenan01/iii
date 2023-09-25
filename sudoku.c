#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pnmrdr.h>
#include <stdbool.h>
#include "uarray2.h"

static int two_power(int power);
static FILE *open_or_fail(char *filename, char *mode);
UArray2_T read_input_file(FILE *file);
void map_to_uarray2(int i, int j, UArray2_T a, void *elem, void *image);
void print_elems(int i, int j, UArray2_T a, void *elem, void *cl);
void solve_col(int i, int j, UArray2_T a, void *elem, void *data);
void solve_row(int i, int j, UArray2_T a, void *elem, void *data);
bool solve_small(UArray2_T array);
bool check_sudoku(UArray2_T sudoku);

struct solution {
        int sum;
        bool success;
};

typedef struct solution solution;

int main(int argc, char *argv[]) {
        assert(argc == 1 || argc == 2);

        bool sudoku_solved = false;

        if (argc == 1) {
                
        } else if (argc == 2) {
                FILE *fp = open_or_fail(argv[1], "rb");
                UArray2_T array = read_input_file(fp);
                sudoku_solved = check_sudoku(array);
                UArray2_free(&array);
                fclose(fp);
        }

        if (sudoku_solved) {
                return EXIT_SUCCESS;
        } else {
                printf("FAIL\n");
                return EXIT_FAILURE;
        }
}

UArray2_T read_input_file(FILE *file) {
        Pnmrdr_T image = Pnmrdr_new(file);
        Pnmrdr_mapdata image_header =  Pnmrdr_data(image);
        assert(image_header.width == 9 && image_header.height == 9);
        assert(image_header.denominator == 9);
        
        UArray2_T array = UArray2_new(9, 9, 4);
        UArray2_map_row_major(array, map_to_uarray2, &image);
        Pnmrdr_free(&image);

        return array;
}

bool check_sudoku(UArray2_T sudoku) {
        solution data;
        data.sum = 0;
        data.success = true;
        UArray2_map_col_major(sudoku, solve_col, &data);
        UArray2_map_row_major(sudoku, solve_row, &data);
        data.success &= solve_small(sudoku);
        
        return data.success;
}

void map_to_uarray2(int i, int j, UArray2_T a, void *elem, void *image) {
        (void) i, (void) j, (void) a;
        *((int*)elem) = Pnmrdr_get(*((Pnmrdr_T*) image));
        assert(*((int*)elem) >= 1);
        assert(*((int*)elem) <= 9);
}

void solve_col(int i, int j, UArray2_T a, void *elem, void *data) {
        (void) a, (void) i;
        solution* curr_data = data;
        int* accum = &curr_data->sum;
        int val = *((int*)elem);

        val = two_power(val);
        *accum += val;

        if (j == 8) {
                curr_data->success = curr_data->success && *accum == 511;
                *accum = 0;
        }
}

void solve_row(int i, int j, UArray2_T a, void *elem, void *data) {
        (void) a, (void) j;
        solution* curr_data = data;
        int* accum = &curr_data->sum;
        int val = *((int*)elem);

        val = two_power(val);
        *accum += val;

        if (i == 8) {
                curr_data->success = curr_data->success && *accum == 511;
                *accum = 0;
        }
}

bool solve_small(UArray2_T array) {
        for (int row = 0; row < 9; row += 3) {
                for (int col = 0; col < 9; col += 3) {
                        int sum = 0;
                        for (int i = row; i < row + 3; i++) {
                                for (int j = col; j < col + 3; j++) {
                                        int val = *(int*)UArray2_at(array, i, j);
                                        sum += two_power(val);
                                }
                        }
                        if (sum != 511){
                                return false;
                        }
                }
        }
        return true;
}

static int two_power(int power) {
        return (1 << (power - 1));
}

void print_elems(int i, int j, UArray2_T a, void *elem, void *cl) {
        (void) a, (void) cl;
        printf("Element at [%d,%d]: %d\n", i, j, *((int*)elem));
}

static FILE *open_or_fail(char *filename, char *mode) {
        FILE *fp = fopen(filename, mode);
        assert(fp);
        return fp;
}