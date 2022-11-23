#include <fstream>
#include <iostream>

using namespace std;

struct BmpHeader {
    char bitmapSignatureBytes[2] = {'B', 'M'};
    uint32_t sizeOfBitmapFile = 54 + 786432;
    uint32_t reservedBytes = 0;
    uint32_t pixelDataOffset = 54;
} bmpHeader;

struct BmpInfoHeader {
    uint32_t sizeOfThisHeader = 40;
    int32_t width = 512; // in pixels
    int32_t height = 512; // in pixels
    uint16_t numberOfColorPlanes = 1; // must be 1
    uint16_t colorDepth = 24;
    uint32_t compressionMethod = 0;
    uint32_t rawBitmapDataSize = 0; // generally ignored
    int32_t horizontalResolution = 3780; // in pixel per meter
    int32_t verticalResolution = 3780; // in pixel per meter
    uint32_t colorTableEntries = 0;
    uint32_t importantColors = 0;
} bmpInfoHeader;

struct Pixel {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
};

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

        this_cell->val = this_cell->x + this_cell->y + this_cell->z;

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

int nearest_cell(cell cells[27], double x, double y, double z){
  cell cell1 = cells[0];
  double dx = cell1.x-x;
  double dy = cell1.y-y;
  double dz = cell1.z-z;
  double dist;
  double near_dist = dx*dx+dy*dy+dz*dz;
  int nearest = 0;
  for (int i = 1; i < 27; i++) {
    this_cell = cells[i];
    dx = this_cell.x-x;
    dy = this_cell.y-y;
    dz = this_cell.z-z;
    dist = dx*dx+dy*dy+dz*dz;
    if (dist<near_dist){
      near_dist = dist;
      nearest = i;
    }
  }
  return nearest;
}

void create_z_slice(int ximax, int etamax, int zetamax, cell *grid, double xmax, double ymax, int w, int h, pixel *img, double z) {

  double x,y;
  double xscale = (xmax-xmin)/w;
  double yscale = (ymax-ymin)/h;

  double last_start_xi;
  double last_start_eta;
  double last_start_zeta;

  //first, find the cell closest to what the top left corner of the screen must represent
  double dx = grid[0].x-xmin;
  double dy = grid[0].y-ymin;
  double dz = grid[0].z-z;
  double near_dist = dx*dx+dy*dy+dz*dz;
  
  for (int xi = 0; xi < ximax; xi++){
    for (int eta = 0; eta < etamax; eta++){
      for (int zeta = 0; zeta < zetamax; zeta++){

      }
    }
  }

  for (int i = 0; i < w; x++) {
    bool started_row = false;
    for (int j = 0; j < h; y++) {
      x = i*xscale+xmin;
      y = j*yscale+ymin;
    }
  }

}

void flood(int, int, int, int);

int main(int argc, char **argv){

  int x,y,z;
  char *filename = argv[1];
  read_grid_dimensions(filename, &x, &y, &z);
  //cout << x << "," << y << "," << z << "\n";

  cell grid[x][y][z];
  double minx,maxx,miny,maxy,minz,maxz;
  fill_grid(filename, x, y, z, &grid[0][0][0], &minx, &maxx, &miny, &maxy, &minz, &maxz);

  //graphics:


  return 0;
}
