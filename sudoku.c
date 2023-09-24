#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pnmrdr.h>
#include <set.h>
#include <atom.h>
#include "uarray2.h"

static FILE *open_or_fail(char *filename, char *mode);
void map_to_uarray2(int i, int j, UArray2_T a, void *elem, void *image);
void print_elems(int i, int j, UArray2_T a, void *elem, void *cl);
void solve_col(int i, int j, UArray2_T a, void *elem, void *set);
void solve_row(int i, int j, UArray2_T a, void *elem, void *set);
void solve_small(int i, int j, UArray2_T a, void *elem, void *set);
void read_input_file(FILE *file);
void valid_dimension(UArray2_T arrray);

int main(int argc, char *argv[]) {
        assert(argc == 1 || argc == 2);

        if (argc == 1) {
                // TO DO
        } else if (argc == 2) {
                FILE *fp = open_or_fail(argv[1], "rb");
                read_input_file(fp);
                fclose(fp);
        }

        return EXIT_SUCCESS;
}

void read_input_file(FILE *file) {
        Pnmrdr_T image = Pnmrdr_new(file);
        Pnmrdr_mapdata image_header =  Pnmrdr_data(image);
        assert(image_header.width == 9);
        assert(image_header.height == 9);
        assert(image_header.denominator == 9);
        
        UArray2_T array = UArray2_new(9, 9, 4);
        UArray2_map_row_major(array, map_to_uarray2, &image);
        //UArray2_map_row_major(array, print_elems, NULL);
        
        valid_dimension(array);
        UArray2_free(&array);
        Pnmrdr_free(&image);
}

void valid_dimension(UArray2_T array) {
        Set_T tmp_set = Set_new(0, NULL, NULL);
        UArray2_map_col_major(array, solve_col, &tmp_set);
        UArray2_map_row_major(array, solve_row, &tmp_set);
        Set_free(&tmp_set);
}

void map_to_uarray2(int i, int j, UArray2_T a, void *elem, void *image) {
        (void) i, (void) j, (void) a;
        *((int*)elem) = Pnmrdr_get(*((Pnmrdr_T*) image));
        assert(*((int*)elem) != 0);
        assert(*((int*)elem) <= 9);
}

void solve_col(int i, int j, UArray2_T a, void *elem, void *set) {
        (void) a, (void) i;
        //printf("[%d, %d]: %d\n", i, j, *(int*)elem);
        const char* atom_int = Atom_int(*(int*)elem);
        Set_put(*((Set_T*) set), atom_int);

        if (j == 8) {
                assert(Set_length(*((Set_T*) set)) == 9);
                Set_free((Set_T*) set);
                *((Set_T*) set) = Set_new(0, NULL, NULL);
        }
}

void solve_row(int i, int j, UArray2_T a, void *elem, void *set) {
        (void) a, (void) j;
        //printf("[%d, %d]: %d\n", i, j, *(int*)elem);
        const char* atom_int = Atom_int(*(int*)elem);
        // could raise memfailed
        Set_put(*((Set_T*) set), atom_int);

        if (i == 8) {
                int length = Set_length(*((Set_T*) set));
                assert(length == 9);
                Set_free((Set_T*) set);
                *((Set_T*) set) = Set_new(0, NULL, NULL);
        }
}

void solve_small(int i, int j, UArray2_T a, void *elem, void *set) {
        (void) i, (void) j, (void) a;
        Set_put(*((Set_T*) set), elem);
        assert(Set_length(*((Set_T*) set)) == j + 1);
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