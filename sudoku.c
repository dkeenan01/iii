#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pnmrdr.h>
#include "uarray2.h"

static FILE *open_or_fail(char *filename, char *mode);
void map_to_uarray2(int i, int j, UArray2_T a, void *elem, void *image);
void print_elems(int i, int j, UArray2_T a, void *elem, void *cl);
void read_input_file(FILE *file);

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
        UArray2_map_row_major(array, print_elems, NULL);

        Pnmrdr_free(&image);
}

void map_to_uarray2(int i, int j, UArray2_T a, void *elem, void *image) {
        (void) i, (void) j, (void) a;
        *((int*)elem) = Pnmrdr_get(*((Pnmrdr_T*) image));
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