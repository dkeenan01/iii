#include <stdlib.h>
#include <stdio.h>
#include "uarray2.h"
#include <assert.h>

int main(int argc, char *argv[]) {
        (void) argc;
        (void) argv;

        UArray2_T test_array;
        test_array = UArray2_new(5, 5, sizeof(int));

        int res_h = UArray2_height(test_array);
        int res_w = UArray2_width(test_array);
        int res_s = UArray2_size(test_array);


        printf("Height %d\n", res_h);
        printf("Width %d\n", res_w);
        printf("Size %d\n", res_s);



        assert ( res_h == 5);
        assert ( res_w == 5);
        assert ( res_s == 4);



        UArray2_free(&test_array);

        return EXIT_SUCCESS;
}