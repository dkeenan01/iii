#ifndef UARRAY2_INCLUDED
#define UARRAY2_INCLUDED
#define A UArray2_T
#include <uarray.h>

#define T UArray2_T

typedef struct T *T;

/********** UArray2_new ********
 *      initializes a new 2d unboxed array with size width*height*size
 *
 * Inputs: width, height of 2d array, and the size of each elem
 *
 * Return: an initalized 2d UArray2
 *
 * Notes: c.r.e if memory cannot be allocated
 *
 ************************/
T UArray2_new(int width, int height, int size);

/********** UArray2_free ********
 *      initializes a new 2d unboxed array with size width*height*size
 *
 * Inputs: the address of the UArray2 to free
 *
 * Notes: returns nothing, frees memory of the UArray2. c.re. for the
 *      UArray2 to be null
 *
 ************************/
void UArray2_free(T *uarray2);

/********** UArray2_height *********
 *       Gets the height of the UArray2 which is the number of rows in the
 *       UArray2.
 *
 *       Input: t  – UArray2 type
 *
 *       Returns: Integer value for the height of parameter array		
 *	 Expectations:
 *		Expects that the array passed in is not null
 * 		The address of the array passed is the same as the instance of 
 *              the UArray2.
*/
int UArray2_height(T uarray2);

/********** UArray2_width *********
 *       Gets the width of the UArray2 which is the number of columns in the
 *       UArray2.
 *
 *       Input: t  – UArray2 type
 *
 *       Returns: Integer value for the width of parameter array
 *		
 *	 Expectations:
 *		Expects that the array passed in is not null
 * 		The address of the array passed is the same as the instance of 
 *              the UArray2.
*/
int UArray2_width(T uarray2);

/********** UArray2_size **********
 *      Gets the size of an element in the UArray2, the number of bytes it 
 *      takes.
 *
 *      Input:
 *              t  – UArray2 type
 *
 *      Returns:
 *              Integer value for the size of each element of the parameter 
 *              UArray2	
 *      
 *      Expectations:
 *              Expects that the array passed in is not null
 *              The address of the array passed is the same as the instance of 
 *              the UArray2.
 */
int UArray2_size(T uarray2);

/********** UArray2_at **********
 *
 *      Returns a pointer to the element at the 2d index (col, row).
 *
 *      Input:
 *              t  – UArray2 type
 *              row  – integer for the row to index element
 *              column – integer for the column to index element
 *
 *      Returns:
 *              Void pointer to the element at the desired (col, row) location 
 *              on the parameterized UArray2
 *      
 *      Expectations:
 *              Expects that the indexes given are not out of range [0, width), 
 *              [0, height)
 *              Expects that the UArray2 passed in is not null
 *              Expects that the address of the UArray2 passed is the same as 
 *              the instance of the UArray2.
 */
void* UArray2_at(T uarray2, int col, int row);

/********** UArray2_map_row_major **********
 *
 *      Applies a function to each element of the UArray2 by going across 
 *      columns first, hitting every element in the row and going down by row.
 *
 *      Input:
 *              t – UArray2 type
 *              An apply function, to perform on each element, whose parameters 
 *              are:
 *                      (col, row) - a 2d index
 *                      t - UArray2 type, a void 
 *                      elem - pointer to the current element
 *                      cl - closure variable to pass information between each 
 *                           call.
 *               cl - closure variable used to pass extra information 
 *                    between each iteration.
 *
 *      Expectations:
 *              Expects that the UArray2 passed in is not null
 *              Expects that the apply function is not null
 *              Expects that the address of the UArray2 passed is the same as 
 *              the instance of the UArray2.
 */
void UArray2_map_row_major(T uarray2, void apply(int col, int row, T uarray2,
        void* elem, void *cl), void *cl);
/********** UArray2_map_row_major **********
 *
 *      Applies a function to each element of the UArray2 by going down 
 *      rows first, hitting every element in the column and going across by 
 *      column.
 *
 *      Input:
 *              t – UArray2 type
 *              An apply function, to perform on each element, whose parameters 
 *              are:
 *                      (col, row) - a 2d index
 *                      t - UArray2 type, a void 
 *                      elem - pointer to the current element
 *                      cl - closure variable to pass information between each 
 *                           call.
 *               cl - closure variable used to pass extra information 
 *                    between each iteration.
 *
 *      Expectations:
 *              Expects that the UArray2 passed in is not null
 *              Expects that the apply function is not null
 *              Expects that the address of the UArray2 passed is the same as 
 *              the instance of the UArray2.
 */
void UArray2_map_col_major(T uarray2, void apply(int col, int row, T uarray2, 
        void* elem, void *cl), void *cl);

#undef T
#endif