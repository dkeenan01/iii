#define T Bit2


void Bit2_new(T t, int width, int height, int size);

void Bit2_free(T t);
int Bit2_length(T t);
int Bit2_width(T t);
int Bit2_size(T t);
int Bit2_get(T t, int n);
int Bit2_put(T t, int n);
void Bit2_map_row_major(T t, void apply(int col, int row, void *cl), void *cl);
void Bit2_map_col_major(T t, void apply(int col, int row, void *cl), void *cl);