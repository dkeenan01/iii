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
Stack_T check_edges(Bit2_T bitmap);
void remove_blackedges(Bit2_T bitmap, Stack_T blackpixel);
bool check_range(point* curr, int max_col, int max_row);
void add_neighbor(Bit2_T bitmap, Stack_T blackpixel, point* p);
void print_bits(int i, int j, Bit2_T bm, int b, void *cl);

int main(int argc, char *argv[]) {
        assert(argc == 1 || argc == 2);
        (void) argv;

        FILE *fp = (argc == 1) ? stdin : open_or_fail(argv[1], "rb");
        Bit2_T bitmap = pbmread(fp);
        Stack_T p_stack = check_edges(bitmap);
        remove_blackedges(bitmap, p_stack);
        FILE *out = fopen("out.pbm", "w");
        Bit2_map_row_major(bitmap, print_bits, out);

        Bit2_free(&bitmap);
        fclose(fp);
        fclose(out);

        return EXIT_SUCCESS;
}

void print_bits(int i, int j, Bit2_T bm, int b, void *cl) 
{
        (void) i, (void) j, (void) bm, (void) cl;
        //FILE* outputfp = cl;
        fprintf(stderr, "%d", b);
        if (i == Bit2_width(bm) - 1) {
                fprintf(stderr, "\n");
        }
}


Stack_T check_edges(Bit2_T bitmap) {
        int width = Bit2_width(bitmap);
        int height = Bit2_height(bitmap);
        Stack_T stack = Stack_new();
        for (int row = 0; row < height; row ++) {
                for (int col = 0; col < width; col ++) {
                        if (col == 0 || row == 0 || col == width -1 || 
                                row == height - 1) {
                                int bit_val = Bit2_get(bitmap, col, row);
                                if (bit_val == 1) {
                                        Bit2_put(bitmap, col, row, 0);
                                        point* curr_point = NEW(curr_point);
                                        curr_point->col = col;
                                        curr_point->row = row;
                                        Stack_push(stack, curr_point);
                                }
                        }
                }
        }
        return stack;
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
Bit2_T pbmread(FILE *file) {
        Pnmrdr_T image = Pnmrdr_new(file);
        Pnmrdr_mapdata image_header =  Pnmrdr_data(image);
        assert(image_header.width != 0 && image_header.height != 0);
        assert(image_header.type == 1);
        
        Bit2_T bitmap = Bit2_new(image_header.width, image_header.height);
        Bit2_map_row_major(bitmap, map_image, &image);
        Pnmrdr_free(&image);

        return bitmap;
}

void remove_blackedges(Bit2_T bitmap, Stack_T blackpixel) {
        while(Stack_empty(blackpixel) == 0) {
                point* curr = Stack_pop(blackpixel);
                point* left_neighbor = NEW(left_neighbor);
                point* right_neighbor = NEW(right_neighbor);
                point* top_neighbor = NEW(top_neighbor);
                point* bottom_neighbor = NEW(bottom_neighbor);
                left_neighbor->col = curr->col - 1;
                left_neighbor->row = curr->row;
                right_neighbor->col = curr->col + 1;
                right_neighbor->row = curr->row;
                top_neighbor->col = curr->col;
                top_neighbor->row = curr->row - 1;
                bottom_neighbor->col = curr->col;
                bottom_neighbor->row = curr->row + 1;
                add_neighbor(bitmap, blackpixel,left_neighbor);
                add_neighbor(bitmap, blackpixel,right_neighbor);
                add_neighbor(bitmap, blackpixel,top_neighbor);
                add_neighbor(bitmap, blackpixel,bottom_neighbor);
                FREE(curr);
        }
        Stack_free(&blackpixel);
}

void add_neighbor(Bit2_T bitmap, Stack_T blackpixel, point* p) {
        int max_col = Bit2_width(bitmap);
        int max_row = Bit2_height(bitmap);
        if (check_range(p, max_col, max_row) && Bit2_get(bitmap, p->col, p->row) == 1) {
                Bit2_put(bitmap, p->col, p->row, 0);
                Stack_push(blackpixel, p);
        } else {
                FREE(p);
        }
}

bool check_range(point* curr, int max_col, int max_row) {
        if (curr->col >= max_col || curr->row >= max_row) {
                return false;
        }

        if (curr->col < 0 || curr->row < 0) {
                return false;
        }

        return true;
}

/*
*               map_image
*
*       details:
*               Gets a pixel value from the Pnmrdr type passed in and writes 
*               that value to the elem parameter. Maps each element of the 
*               image file to a Bit2 when map row major is called using this 
*               mapping function.
*       inputs:
*               i - not used (used to match function params)
*               j - not used (used to match function params)
*               a - not used (used to match function params)
*               int n - the current value fo the bit at i, j
*               image - void pointer really pointer to Pnmrdr_T type to get 
*                       file contents from.
*       
*/
void map_image(int i, int j, Bit2_T bm, int n, void *image) {
        (void) n;
        int value = Pnmrdr_get(*((Pnmrdr_T*) image));
        Bit2_put(bm, i, j, value);
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