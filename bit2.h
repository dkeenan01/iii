#ifndef BIT2_INCLUDED
#define BIT2_INCLUDED
#define T Bit2_T

typedef struct T *T;

/********** Bit2_new ********
 *      initializes a new 2d bit array with size width*height
 *
 * Inputs: width, height of 2d array
 *
 * Return: an initalized 2d bit array
 *
 * Notes: c.r.e if memory cannot be allocated
 *
 ************************/
T Bit2_new(int width, int height);

/********** Bit2_free ********
 *      Frees the memory associated with a Bit2
 *
 * Inputs: the address of the Bit2 to free
 *
 * Notes: returns nothing
 *        c.r.e. for the UArray2 to be null
 *
 ************************/
void Bit2_free(T* bit2);

/********** Bit2_height *********
 *       Gets the height of the Bit2 which is the number of rows in the
 *       Bit2.
 *
 *       Input: bit2  – Bit2 type
 *
 *       Returns: Integer value for the height of parameter array
 *		
 *	 Expectations:
 *		Expects that the array passed in is not null
 * 		The address of the array passed is the same as the instance of 
 *              the Bit2.
*/
int Bit2_height(T bit2);

/********** Bit2_width *********
 *       Gets the width of the Bit2 which is the number of columns in the
 *       Bit2.
 *
 *       Input: bit2  – Bit2 type
 *
 *       Returns: Integer value for the width of parameter array
 *		
 *	 Expectations:
 *		Expects that the array passed in is not null
 * 		The address of the array passed is the same as the instance of 
 *              the Bit2.
*/
int Bit2_width(T bit2);

/********** Bit2_get **********
 *
 *      gets the value of the bit at (col, row) in the array.
 *
 *      Input:
 *              bit2  – Bit2 type
 *              row  – integer for the row to index element
 *              column – integer for the column to index element
 *
 *      Returns:
 *              Integer for the value of the bit at (col, row)
 *      
 *      Expectations:
 *              Expects that the indexes given are not out of range [0, width), 
 *              [0, height)
 *              Expects that the Bit2 passed in is not null
 *              Expects that the address of the Bit2 passed is the same as 
 *              the instance of the Bit2.
 */
int Bit2_get(T bit2, int col, int row);

/********** Bit2_put **********
 *
 *      sets the value of the bit at (col, row) in the array.
 *
 *      Input:
 *              bit2  – Bit2 type
 *              row  – integer for the row to index element
 *              column – integer for the column to index element
 *              n - int for the value to set the bit to
 *
 *      Returns:
 *              Integer for the value of the bit at (col, row) that was there 
 *              before
 *      
 *      Expectations:
 *              Expects that the indexes given are not out of range [0, width), 
 *              [0, height)
 *              Expects that the Bit2 passed in is not null
 *              Expects that the address of the Bit2 passed is the same as 
 *              the instance of the Bit2.
 */
int Bit2_put(T bit2, int col, int row, int n);

/********** Bit2_map_row_major **********
 *
 *      Applies a function to each element of the Bit2 by going across 
 *      columns first, hitting every element in the row and going down by row.
 *
 *      Input:
 *              t – Bit2 type
 *              An apply function, to perform on each element, whose parameters 
 *              are:
 *                      (col, row) - a 2d index
 *                      t -  Bit2 type, a void pointer
 *                      n - integer for bit value
 *                      cl - closure variable to pass information between each 
 *                           call.
 *               cl - closure variable used to pass extra information 
 *                    between each iteration.
 *
 *      Expectations:
 *              Expects that the Bit2 passed in is not null
 *              Expects that the apply function is not null
 *              Expects that the address of the Bit2 passed is the same as 
 *              the instance of the Bit2.
 */
void Bit2_map_row_major(T bit2, void apply(int col, int row, T bit2, int b,
        void *cl), void *cl);

/********** Bit2_map_col_major **********
 *
 *      Applies a function to each element of the Bit2 by going down 
 *      rows first, hitting every element in the column and going accross by 
 *      column.
 *
 *      Input:
 *              t – Bit2 type
 *              An apply function, to perform on each element, whose parameters 
 *              are:
 *                      (col, row) - a 2d index
 *                      t - Bit2 type, a void pointer
 *                      n - integer for bit value
 *                      cl - closure variable to pass information between each 
 *                           call.
 *               cl - closure variable used to pass extra information 
 *                    between each iteration.
 *
 *      Expectations:
 *              Expects that the Bit2 passed in is not null
 *              Expects that the apply function is not null
 *              Expects that the address of the Bit2 passed is the same as 
 *              the instance of the Bit2.
 */

void Bit2_map_col_major(T bit2, void apply(int col, int row, T bit2, int b,
        void *cl), void *cl);

#undef T
#endif