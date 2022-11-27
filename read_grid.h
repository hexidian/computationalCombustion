#ifndef H_READ_GRID
#define H_READ_GRID

struct cell;

int read_grid_dimensions(char *filename, int *x, int *y, int *z);

int fill_grid(char *filename, int x, int y, int z, cell *grid, double *minx, double *maxx, double *miny, double *maxy, double *minz, double *maxz);

#endif
