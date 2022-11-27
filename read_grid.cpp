#include <fstream>
#include <iostream>

using namespace std;

struct cell {
  double x;
  double y;
  double z;
  double val;
};

int read_grid_dimensions(char *filename, int *x, int *y, int *z) {

  ifstream rf(filename, ios::out | ios::binary);
  if(!rf) {
    cout << "can't read file to get dimensions\n";
    return 1;
  }
  char header;
  rf.read(&header, sizeof(char));
  rf.read((char *) x, sizeof(int));
  rf.read((char *) y, sizeof(int));
  rf.read((char *) z, sizeof(int));

  if(!rf.good()){
    cout << "error while reading dimensions\n";
    return 1;
  }

  rf.close();
  if(!rf.good()) {
    cout << "there was an error while reading dimensions\n";
    return 1;
  }

  return 0;
}

int fill_grid(char *filename, int x, int y, int z, cell *grid, double *minx, double *maxx, double *miny, double *maxy, double *minz, double *maxz) {

  ifstream rf(filename, ios::out | ios::binary);
  if(!rf) {
    cout << "can't read file to get dimensions\n";
    return 1;
  }

  char header;
  int x_,y_,z_;

  rf.read(&header, sizeof(char));
  rf.read((char *) &x_, sizeof(int));
  rf.read((char *) &y_, sizeof(int));
  rf.read((char *) &z_, sizeof(int));

  if (! (x==x_ && y==y_ && z==z_)) {
    cout << "dimensions do not match with file\n";
    return 1;
  }

  for (int xi = 0; xi < x; xi++) {
    for (int eta = 0; eta < y; eta ++) {
      for (int zeta = 0; zeta < z; zeta++) {

        cell *this_cell = &grid[xi*y*z+eta*z+zeta];

        rf.read((char *) &this_cell->x,sizeof(double));
        rf.read((char *) &this_cell->y,sizeof(double));
        rf.read((char *) &this_cell->z,sizeof(double));

        this_cell->val = 50*this_cell->y;// + this_cell->y + this_cell->z;

        if (this_cell->x < *minx) *minx = this_cell->x;
        else if (this_cell->x > *maxx) *maxx = this_cell->x;
        if (this_cell->y < *miny) *miny = this_cell->y;
        else if (this_cell->y > *maxy) *maxy = this_cell->y;
        if (this_cell->z < *minz) *minz = this_cell->z;
        else if (this_cell->z > *maxz) *maxz = this_cell->z;

      }
    }
  }

  rf.close();
  if(!rf.good()) {
    cout << "there was an error reading grid coords";
    return 1;
  }

  return 0;
}
