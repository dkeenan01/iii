/**************************************************************
 *
 *                     sudoku.c
 *
 *     Assignment: hw2 iii
 *     Authors:  Dylan Keenan (dkeena01), Cooper Golemmme (cgolem01)
 *     Date:     09/28/2023
 *
 *     implements an program that checks for a valid sudoku solution
 *     from a pgm file. 
 *
 **************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pnmrdr.h>
#include <stdbool.h>
#include "uarray2.h"

static int two_power(int power);
static FILE *open_or_fail(char *filename, char *mode);
UArray2_T read_input_file(FILE *file);
void map_image(int i, int j, UArray2_T a, void *elem, void *image);
void print_elems(int i, int j, UArray2_T a, void *elem, void *cl);
void solve_col(int i, int j, UArray2_T a, void *elem, void *data);
void solve_row(int i, int j, UArray2_T a, void *elem, void *data);
bool incorrect_3x3(UArray2_T array, int row, int col);
bool solve_small(UArray2_T array);
bool check_sudoku(UArray2_T sudoku);

struct solution {
        int sum;
        bool success;
}; 

typedef struct solution solution;

int main(int argc, char *argv[]) 
{
        assert(argc == 1 || argc == 2);

        FILE *fp = (argc == 1) ? stdin : open_or_fail(argv[1], "rb");
        UArray2_T array = read_input_file(fp);
        bool sudoku_solved = false;
        if (UArray2_width(array) == 9) {
                sudoku_solved = check_sudoku(array);
        }

        UArray2_free(&array);
        fclose(fp);

        return (sudoku_solved) ? EXIT_SUCCESS : EXIT_FAILURE;
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
UArray2_T read_input_file(FILE *file) 
{
        Pnmrdr_T image = Pnmrdr_new(file);
        Pnmrdr_mapdata image_header =  Pnmrdr_data(image);
        assert(image_header.type == 2);

        UArray2_T array;
        if(image_header.width == 9 && image_header.height == 9 
                && image_header.denominator == 9) {
                array = UArray2_new(9, 9, 4);
                UArray2_map_row_major(array, map_image, &image);
        } else {
                array = UArray2_new(0,0,1);
        }

        Pnmrdr_free(&image);
        return array;
}

/*
*               check_sudoku
*
*       details:
*               Runs tests to validate a sudoku solution. Checks that the rows 
*               and columns contain the unique elements 0-9 and that the 
*               subsquares do as well.
*       inputs:
*               UArray2_T sudoku - Uarray2 with sudoku data in it.
*       returns:
*               True if Uarray2 is a valid sudoku solution
*/
bool check_sudoku(UArray2_T sudoku) 
{
        solution data;
        data.sum = 0;
        data.success = true;

        UArray2_map_col_major(sudoku, solve_col, &data);
        UArray2_map_row_major(sudoku, solve_row, &data);
        data.success = data.success && solve_small(sudoku);

        return data.success;
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
*               image - void pointer points to Pnm reader with the image
*
*/
void map_image(int i, int j, UArray2_T a, void *elem, void *image) 
{
        (void) i, (void) j, (void) a;
        *((int*)elem) = Pnmrdr_get(*((Pnmrdr_T*) image));
}

/*
*               solve_col
*
*       details:
*               Checks that the columns in the image contain all numbers 1-9
*       inputs:
*               i - not used (used to match function params)
*               j - int to check if program reaches end of column
*               a - not used (used to match function params)
*               elem - void pointer that is an integer pointer to get that 
*                       the value of that element of the UArray2
*               data - void pointer really pointer to a solution struct used to
*                      validate solution.
*       
*/
void solve_col(int i, int j, UArray2_T a, void *elem, void *data) 
{
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

/*
*               solve_row
*
*       details:
*               Checks that the rows in the image contain all numbers 1-9
*       inputs:
*               i - int to check if program reaches end of row
*               j - not used (used to match function params)
*               a - not used (used to match function params)
*               elem - void pointer that is an integer pointer to get that 
*                       the value of that element of the UArray2
*               data - void pointer really pointer to a solution struct used to
*                      validate solution.
*       
*/
void solve_row(int i, int j, UArray2_T a, void *elem, void *data) 
{
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

/*
*               solve_small
*
*       details:
*               checks if the smaller 3x3 subsquares of 9x9 sodoku contain all 
*               numbers 1-9.
*       inputs:
*               array - UArray2 to run the check on.
*       returns:
*               true if the subsquares are valid
*/
bool solve_small(UArray2_T array) 
{
        for (int row = 0; row < 9; row += 3) {
                for (int col = 0; col < 9; col += 3) {
                        if (incorrect_3x3(array, row, col)){
                                return false;
                        }
                }
        }
        return true;
}


bool incorrect_3x3(UArray2_T array, int row, int col)
{
        int sum = 0;
        for (int i = row; i < row + 3; i++) {
                for (int j = col; j < col + 3; j++) {
                        int val = *(int*)UArray2_at(array, i, j);
                        sum += two_power(val);
                }
        }
        return sum != 511;
}

/*
*               two_power
*      
*       details:
*               computes the 2^power-1 using bitwise operations
*       input:
*               power - int to raise 2 to.
*       returns:
*               int for 2^(power - 1)
*       Exceptions:
*               could raise excpetion if 2^(power-1) exceeds 32 bit int limit
*/
static int two_power(int power) 
{
        return (1 << (power - 1));
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
static FILE *open_or_fail(char *filename, char *mode) 
{
        FILE *fp = fopen(filename, mode);
        assert(fp != NULL);
        return fp;
}