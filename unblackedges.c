/**************************************************************
 *
 *                     unblackedges.c
 *
 *     Assignment: hw2 iii
 *     Authors:  Dylan Keenan (dkeena01), Cooper Golem (cgolem01)
 *     Date:     09/28/2023
 *
 *     implements an program that removes black edges pixels from
 *     a pbm file and outputs the new image to standard output
 *
 **************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pnmrdr.h>
#include <stdbool.h>
#include <mem.h>
#include <stack.h>
#include "bit2.h"

struct point {
        int col;
        int row;
};

typedef struct point point;

static FILE *open_or_fail(char *filename, char *mode);
Bit2_T pbmread(FILE* inputfp);
void pbmwrite(FILE* outputfp, Bit2_T bitmap);
void map_image(int i, int j, Bit2_T bitmap, int b, void *image);
void check_edges(int col, int row, Bit2_T bitmap, int b, void *stack);
void remove_blackedges(Bit2_T bitmap);
bool check_range(int i, int j, int max_col, int max_row);
void add_neighbor(Bit2_T bitmap, Stack_T blackpixel, int col, int row);
void print_bits(int i, int j, Bit2_T bm, int b, void *cl);

int main(int argc, char *argv[])
{
        assert(argc == 1 || argc == 2);

        /* open/close file - file data in bit2 */
        FILE *fp = (argc == 1) ? stdin : open_or_fail(argv[1], "rb");
        Bit2_T bitmap = pbmread(fp);
        fclose(fp);

        /* get all black edge pixels and remove them */
        remove_blackedges(bitmap);

        /* output corrected image and free its storage */
        pbmwrite(stdout, bitmap);
        Bit2_free(&bitmap);

        return EXIT_SUCCESS;
}

/********** pbmwrite ********
 *      write a pbm image to a given output
 *
 * Inputs: a pointer to the output file, the image raster as a bit2
 *
 * Notes: returns nothing, used only to print the image
 *
 ************************/
void pbmwrite(FILE* outputfp, Bit2_T bitmap)
{
        fprintf(outputfp, "P1\n");
        int width = Bit2_width(bitmap);
        int height = Bit2_height(bitmap);
        fprintf(outputfp, "%d %d\n", width, height);
        Bit2_map_row_major(bitmap, print_bits, outputfp);
}

/********** print_bits ********
 *      prints the rows of the pbm image raster
 *
 * Inputs: 2d index (i, j), bit2 image raster, current value of the bit
 *      at i, j, void pointer to the output file
 *
 * Notes: returns nothing, used with bit2 map_row_major to set values
 *
 ************************/
void print_bits(int i, int j, Bit2_T bm, int b, void *cl) 
{
        (void) j;
        FILE* outputfp = cl;
        if (i == Bit2_width(bm) - 1) {
                fprintf(outputfp, "%d\n", b);
        } else {
                fprintf(outputfp, "%d ", b);
        }
}

/********** check_edges ********
 *      changes black border pixels to white, adds pixels to stack to
 *              check its neighbors
 *
 * Inputs: a 2d index (col, row), bit2 image raster, current value of 
 *      the bit at (col, row), void pointer to a stack for neighbor check
 *
 * Notes: returns nothing, used with bit2 map_row_major get black edges
 *
 ************************/
void check_edges(int col, int row, Bit2_T bitmap, int b, void *stack) {
        Stack_T edgepixels = stack;
        int width = Bit2_width(bitmap);
        int height = Bit2_height(bitmap);
        if (col == 0 || row == 0 || col == width -1 || row == height - 1) {
                if (b == 1) {
                        Bit2_put(bitmap, col, row, 0);
                        point* pixel = NEW(pixel);
                        *pixel = (point) {col, row};
                        Stack_push(edgepixels, pixel);
                }
        }
}

/********** remove_blackedges ********
 *      sets any black edges pixel to white and checks neighbors
 * 
 * Inputs: an image raster as a bit2, a stack to hold black edges pixels
 *
 * Notes:
 *      returns nothing, frees the stack its given but not bit2
 ************************/
void remove_blackedges(Bit2_T bitmap)
{
        Stack_T edgepixels = Stack_new();
        Bit2_map_row_major(bitmap, check_edges, edgepixels);

        while(Stack_empty(edgepixels) == 0) {
                point* curr = Stack_pop(edgepixels);
                add_neighbor(bitmap, edgepixels, curr->col - 1, curr->row);
                add_neighbor(bitmap, edgepixels, curr->col + 1, curr->row);
                add_neighbor(bitmap, edgepixels, curr->col, curr->row - 1);
                add_neighbor(bitmap, edgepixels, curr->col, curr->row + 1);
                FREE(curr);
        }
        Stack_free(&edgepixels);
}

/********** pbmread ********
 *      Open file with Pnmrdr reader and create bit2 with pixel data
 *
 * Inputs: the pbm image file to be read from
 *
 * Returns: the image raster as a bit2
 *
 * Notes: Hanson assertion if the image height and width are 0 or if 
 *      the type of image in not a pbm.
 ************************/
Bit2_T pbmread(FILE *file)
{
        Pnmrdr_T image = Pnmrdr_new(file);
        Pnmrdr_mapdata image_header =  Pnmrdr_data(image);
        assert(image_header.width != 0 && image_header.height != 0);
        assert(image_header.type == 1);
        
        Bit2_T bitmap = Bit2_new(image_header.width, image_header.height);
        Bit2_map_row_major(bitmap, map_image, &image);
        Pnmrdr_free(&image);

        return bitmap;
}



/********** add_neighbor ********
 *      sets a black edge pixel to white and adds it to the stack
 * 
 * Inputs: an image raster as a bit2, a stack to hold black edges pixels,
 *      and the 2d index of the current pixels
 *
 * Notes:
 *      returns nothing, frees the stack its given but not bit2
 ************************/
void add_neighbor(Bit2_T bitmap, Stack_T blackpixel, int col, int row) 
{
        int max_col = Bit2_width(bitmap);
        int max_row = Bit2_height(bitmap);

        if (check_range(col, row, max_col, max_row) 
                && Bit2_get(bitmap, col, row) == 1) {
                point* pixel = NEW(pixel);
                *pixel = (point) { col, row };
                Bit2_put(bitmap, pixel->col, pixel->row, 0);
                Stack_push(blackpixel, pixel);
        }
}

/********** check_range ********
 *      checks if a 2d index in range given a max
 * 
 * Inputs: a 2d index col, row and the max for each dimension
 *
 * Returns: a boolean, true if in range, false otherwise
 *
 ************************/
bool check_range(int i, int j, int max_col, int max_row) 
{
        if (i >= max_col || j >= max_row || i < 0 || j < 0) {
                return false;
        }
        return true;
}

/********** map_image ********
 *      Uses pnm reader to get image raster values and store in bit2
 * 
 * Inputs: a 2d index (i, j), bit2 image raster, current value of the bit
 *      at i, j (not used), void pointer to pnm reader with the image
 *
 * Notes: returns nothing, used with bit2 map_row_major to set values
 *
 ************************/
void map_image(int i, int j, Bit2_T bm, int n, void *image) 
{
        (void) n;
        int value = Pnmrdr_get(*((Pnmrdr_T*) image));
        Bit2_put(bm, i, j, value);
}

/********** map_image ********
 *      opens file or aborts program
 * 
 * Inputs: a cstring filename, and the mode to open the file with
 *
 * Returns a pointer to the open file
 *
 * Notes: c.r.e if filename could not be opened.
 *
 ************************/
static FILE *open_or_fail(char *filename, char *mode) 
{
        FILE *fp = fopen(filename, mode);
        assert(fp);
        return fp;
}