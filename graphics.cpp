#include <fstream>
#include <iostream>
#include "read_grid.h"

using namespace std;

struct BmpHeader {
    char bitmapSignatureBytes[2] = {'B', 'M'};
    uint32_t sizeOfBitmapFile = 54 + 270000;
    uint32_t reservedBytes = 0;
    uint32_t pixelDataOffset = 54;
} bmpHeader;

struct BmpInfoHeader {
    uint32_t sizeOfThisHeader = 40;
    int32_t width = 300; // in pixels
    int32_t height = -300; // in pixels
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

/*
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
*/
int nearest_cell(cell cells[27], double x, double y, double z){
  cell cell1 = cells[0];
  double dx = cell1.x-x;
  double dy = cell1.y-y;
  double dz = cell1.z-z;
  double dist;
  double near_dist = dx*dx+dy*dy+dz*dz;
  int nearest = 0;
  cell this_cell;
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

double update_nearest_cell(int ximax, int etamax, int zetamax, cell *grid, int *xi_, int *eta_, int *zeta_, double x, double y, double z) {
  //returns dist^2

  int xi = *xi_;
  int eta = *eta_;
  int zeta = *zeta_;

  double near_dist,dx,dy,dz;
  cell this_cell = grid[xi*etamax*zetamax + eta*zetamax + zeta];
  dx = this_cell.x-x;
  dy = this_cell.y-y;
  dz = this_cell.z-z;
  near_dist = dx*dx+dy*dy+dz*dz;

  int near_xi = xi;
  int near_eta = eta;
  int near_zeta = zeta;

  double dist;
  if (xi!=0) {
    this_cell = grid[(xi-1)*etamax*zetamax + eta*zetamax + zeta];
    dx = this_cell.x-x;
    dy = this_cell.y-y;
    dz = this_cell.z-z;
    dist = dx*dx+dy*dy+dz*dz;

    if (dist<near_dist) {
      near_dist = dist;
      near_xi = xi-1;
      near_eta = eta;
      near_zeta = zeta;
    }
  }
  if (xi!=ximax-1) {
    this_cell = grid[(xi+1)*etamax*zetamax + eta*zetamax + zeta];
    dx = this_cell.x-x;
    dy = this_cell.y-y;
    dz = this_cell.z-z;
    dist = dx*dx+dy*dy+dz*dz;

    if (dist<near_dist) {
      near_dist = dist;
      near_xi = xi+1;
      near_eta = eta;
      near_zeta = zeta;
    }
  }

  if (eta!=0) {
    this_cell = grid[xi*etamax*zetamax + (eta-1)*zetamax + zeta];
    dx = this_cell.x-x;
    dy = this_cell.y-y;
    dz = this_cell.z-z;
    dist = dx*dx+dy*dy+dz*dz;

    if (dist<near_dist) {
      near_dist = dist;
      near_xi = xi;
      near_eta = eta-1;
      near_zeta = zeta;
    }
  }
  if (eta!=etamax-1) {
    this_cell = grid[xi*etamax*zetamax + (eta+1)*zetamax + zeta];
    dx = this_cell.x-x;
    dy = this_cell.y-y;
    dz = this_cell.z-z;
    dist = dx*dx+dy*dy+dz*dz;

    if (dist<near_dist) {
      near_dist = dist;
      near_xi = xi;
      near_eta = eta+1;
      near_zeta = zeta;
    }
  }

  if (zeta!=0) {
    this_cell = grid[xi*etamax*zetamax + eta*zetamax + zeta-1];
    dx = this_cell.x-x;
    dy = this_cell.y-y;
    dz = this_cell.z-z;
    dist = dx*dx+dy*dy+dz*dz;

    if (dist<near_dist) {
      near_dist = dist;
      near_xi = xi;
      near_eta = eta;
      near_zeta = zeta-1;
    }
  }
  if (zeta!=zetamax-1) {
    this_cell = grid[xi*etamax*zetamax + eta*zetamax + zeta+1];
    dx = this_cell.x-x;
    dy = this_cell.y-y;
    dz = this_cell.z-z;
    dist = dx*dx+dy*dy+dz*dz;

    if (dist<near_dist) {
      near_dist = dist;
      near_xi = xi;
      near_eta = eta;
      near_zeta = zeta+1;
    }
  }

  *xi_ = near_xi;
  *eta_ = near_eta;
  *zeta_ = near_zeta;

  return near_dist;

}

void create_z_slice(int ximax, int etamax, int zetamax, cell *grid, double xmax, double xmin, double ymax, double ymin, int w, int h, Pixel *img, double z) {

  double x,y;
  double xrange = xmax-xmin;
  double yrange = ymax-ymin;
  if (xrange > yrange) yrange = xrange;
  else xrange = yrange;
  double xscale = (xrange)/w;
  double yscale = (yrange)/h;

  double last_start_xi;
  double last_start_eta;
  double last_start_zeta;

  //first, find the cell closest to what the top left corner of the screen must represent
  double dx = grid[0].x-xmin;
  double dy = grid[0].y-ymin;
  double dz = grid[0].z-z;
  double dist = dx*dx+dy*dy+dz*dz;
  double near_dist = dist;
  int nearest_xi = 0;
  int nearest_eta = 0;
  int nearest_zeta = 0;

  cell this_cell;

  //find the nearest cell the starting point in the top left corner
  for (int xi = 0; xi < ximax; xi++){
    for (int eta = 0; eta < etamax; eta++){
      for (int zeta = 0; zeta < zetamax; zeta++){
        this_cell = grid[xi*etamax*zetamax + eta*zetamax + zeta];
        dx = this_cell.x-xmin;
        dy = this_cell.y-ymin;
        dz = this_cell.z-z;
        dist = dx*dx+dy*dy+dz*dz;
        if (dist < near_dist) {
          near_dist = dist;
          nearest_xi = xi;
          nearest_eta = eta;
          nearest_zeta = zeta;
        }
      }
    }
  }

  last_start_xi = nearest_xi;
  last_start_eta = nearest_eta;
  last_start_zeta = nearest_zeta;
  bool started_row;
  int red,blue,green,val;

  double min_dist = 0.001;

  for (int i = 0; i < w; i++) {
    started_row = false;
    nearest_xi = last_start_xi;
    nearest_eta = last_start_eta;
    nearest_zeta = last_start_zeta;
    //cout << "\nrow: " << i << " " << nearest_zeta << "\n";
    for (int j = 0; j < h; j++) {
      x = i*xscale+xmin;
      y = j*yscale+ymin;
      dist = update_nearest_cell(ximax, etamax, zetamax, grid,
        &nearest_xi, &nearest_eta, &nearest_zeta, x, y, z);

      if (dist > min_dist) {
        near_dist = dist;
        for (int xi = 0; xi < ximax; xi++){
          for (int eta = 0; eta < etamax; eta++){
            for (int zeta = 0; zeta < zetamax; zeta++){
              this_cell = grid[xi*etamax*zetamax + eta*zetamax + zeta];
              dx = this_cell.x-x;
              dy = this_cell.y-y;
              dz = this_cell.z-z;
              dist = dx*dx+dy*dy+dz*dz;
              if (dist < near_dist) {
                near_dist = dist;
                nearest_xi = xi;
                nearest_eta = eta;
                nearest_zeta = zeta;
              }
            }
          }
        }

        if (near_dist > min_dist){
          img[i*w+j].red = 0;
          img[i*w+j].green = 0;
          img[i*w+j].blue = 0;
          continue;
        }

      }

      if (j==0) {
        last_start_xi = nearest_xi;
        last_start_eta = nearest_eta;
        last_start_zeta = nearest_zeta;
      }

      this_cell = grid[nearest_xi*etamax*zetamax + nearest_eta*zetamax + nearest_zeta];
      val = ((int) this_cell.val) * 10;
      //val = nearest_zeta*3;

      if (val >= 0) red=255;
      else if (val < -255) red=0;
      else red=255+val;
      if (val <= 0) blue=255;
      else if (val > 255) blue=0;
      else blue=255-val;
      green = red+blue-255;

      /*
      red = 10*nearest_xi;
      if (red>255) red=255;
      green = 10*nearest_eta;
      if (green>255) green=255;
      blue = 10*nearest_zeta;
      if (blue>255) blue=255;
      */
      img[i*w+j].red = red;
      img[i*w+j].green = green;
      img[i*w+j].blue = blue;

      // if (nearest_zeta==20) {
      //   cout << "\nhere at i,j= " << i << "," << j << "\n";
      // }

    }

  }

}

int main(int argc, char **argv){

  int x,y,z;
  char *filename = argv[1];
  read_grid_dimensions(filename, &x, &y, &z);
  cout << x << "," << y << "," << z << "\n";

  cell grid[x][y][z];
  double minx,maxx,miny,maxy,minz,maxz;
  fill_grid(filename, x, y, z, &grid[0][0][0], &minx, &maxx, &miny, &maxy, &minz, &maxz);

  //graphics:

  int h = 300;
  int w = 300;
  Pixel img[w][h];
  //cout << "\nminx: " << minx << "\nmaxx: " << maxx << "\n\n";
  create_z_slice(x,y,z,&grid[0][0][0],maxx,minx,maxy,miny,w,h,&img[0][0],(double)0.06);

  ofstream fout("image.bmp", ios::binary);

  //write the enttries in bmpHeader in a non-compiler-dependent way
  fout.write((char *) &bmpHeader.bitmapSignatureBytes, 2);
  fout.write((char *) &bmpHeader.sizeOfBitmapFile, sizeof(uint32_t));
  fout.write((char *) &bmpHeader.reservedBytes, sizeof(uint32_t));
  fout.write((char *) &bmpHeader.pixelDataOffset, sizeof(uint32_t));

  fout.write((char *) &bmpInfoHeader.sizeOfThisHeader, sizeof(uint32_t));
  fout.write((char *) &bmpInfoHeader.width, sizeof(int32_t));
  fout.write((char *) &bmpInfoHeader.height, sizeof(int32_t));
  fout.write((char *) &bmpInfoHeader.numberOfColorPlanes, sizeof(uint16_t));
  fout.write((char *) &bmpInfoHeader.colorDepth, sizeof(uint16_t));
  fout.write((char *) &bmpInfoHeader.compressionMethod, sizeof(uint32_t));
  fout.write((char *) &bmpInfoHeader.rawBitmapDataSize, sizeof(uint32_t));
  fout.write((char *) &bmpInfoHeader.horizontalResolution, sizeof(int32_t));
  fout.write((char *) &bmpInfoHeader.verticalResolution, sizeof(int32_t));
  fout.write((char *) &bmpInfoHeader.colorTableEntries, sizeof(uint32_t));
  fout.write((char *) &bmpInfoHeader.importantColors, sizeof(uint32_t));

  //fout.write((char *) &bmpHeader, 14);
  //fout.write((char *) &bmpInfoHeader, 40);

  int pixel_count = w*h;
  Pixel *img_ = &img[0][0];
  for (int i = 0; i<pixel_count; i++) {
    fout.write((char *) &img_[i].blue, 1);
    fout.write((char *) &img_[i].green, 1);
    fout.write((char *) &img_[i].red, 1);
  }
  fout.close();

  return 0;
}
