#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pnmrdr.h>
#include "uarray2.h"

const int 

static FILE *open_or_fail(char *filename, char *mode);

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
        assert(image_header.width == 9)

        printf("Image width: %u\n",image_header.width);
        printf("Image height: %u\n",image_header.height);
        printf("Image denominator: %u\n",image_header.denominator);
}

static FILE *open_or_fail(char *filename, char *mode) {
        FILE *fp = fopen(filename, mode);
        assert(fp);
        return fp;
}