#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pnmrdr.h>
#include <stdbool.h>
#include "bit2.h"

static FILE *open_or_fail(char *filename, char *mode);
Bit2_T pbmread(FILE* inputfp);
void pbmwrite(FILE* outputfp, Bit2_T bitmap);
void map_image(int i, int j, Bit2_T bitmap, int b, void *image);

int main(int argc, char *argv[]) {
        assert(argc == 1 || argc == 2);

        char *filename;
        FILE *fp = (argc == 1) ? stdin : open_or_fail(argv[1], 'rb');
        Bit2_T bitmap = pbmread(fp);

        Bit2_free(&array);
        fclose(fp);

        return EXIT_SUCCESS;
}

/*
*               read_input_file
*
*       details:
*               Open file with Pnmrdr reader and create Uarray2 with pixel data
*               Hanson assertion if the image height and width are not 9
*               or if the max_val of the pixels is not 9.
*       inputs:
*               FILE type to read from
*       returns:
*               UArray2_T with pixel data set
*/
Bit2_T read_input_file(FILE *file) {
        Pnmrdr_T image = Pnmrdr_new(file);
        Pnmrdr_mapdata image_header =  Pnmrdr_data(image);
        assert(image_header.width != 0 && image_header.height != 0);
        assert(image.type == 1);
        
        Bit2_T bitmap = Bit2_new(image_header.width, image_header.height);
        Bit2_map_row_major(bitmap, map_image, &image);
        Pnmrdr_free(&image);

        return bitmap;
}


/*
*               map_to_uarray2
*
*       details:
*               Gets a pixel value from the Pnmrdr type passed in and writes 
*               that value to the elem parameter. Maps each element of the 
*               image file to a Uarray2 when map row major is called using this 
*               mapping function.
*       inputs:
*               i - not used (used to match function params)
*               j - not used (used to match function params)
*               a - not used (used to match function params)
*               elem - void pointer that is an integer pointer to set that 
*                       element to value read from image.
*               image - void pointer really pointer to Pnmrdr_T type to get 
*                       file contents from.
*       
*/
void map_to_uarray2(int i, int j, UArray2_T a, void *elem, void *image) {
        (void) i, (void) j, (void) a;
        *((int*)elem) = Pnmrdr_get(*((Pnmrdr_T*) image));
}


/*
*               open_or_fail
*
*       details:
*               opens file or aborts program
*       inputs:
*               filename - character pointer, filename to open
*               mode - mode to open file with (Ex: "rb")
*       outputs:
*               pointer to the opened FILE
*       Raises Hanson assertion if filename could not be opened.
*       
*/
static FILE *open_or_fail(char *filename, char *mode) {
        FILE *fp = fopen(filename, mode);
        assert(fp);
        return fp;
}