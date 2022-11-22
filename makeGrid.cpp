#include <iostream>
#include <fstream>
#include <math.h>
#include <string.h>

#define PI 3.14159265

using namespace std;

struct coord3d {
  double x;
  double y;
  double z;
};

int str2int(char *str)
{
  int len = strlen(str);
  int val = 0;
  for (int i = 0; i < len; i++){
    val += (str[i]-'0')*round(pow(10,len-i-1));
    //cout << str[i] << '\n';
  }

  return val;
}

int write_grid(int x, int y, int z, coord3d *grid, char boundary_type)
{
  /*boundary_type:  "a" = annulus i.e. the top zone around the fuel annulus
                    "r" = recirculation i.e. further from the annulus but in the top (more course grid)
                    "i" = impingement i.e. the top of the lox ring down to
                                          the bottom of the pintle
                    "c" = combustion chamber i.e. from the pintle down to
                                          the converging section
                    "n" = nozzle i.e. the converging/diverging section
  */
  ofstream wf("grid.lgf", ios::out | ios::binary);
  if (!wf) {
    cout << "cannot open file\n";
    return 1;
  }
  char header = 'c';
  wf.write(&header, sizeof(char));
  wf.write((char *) &x, sizeof(int));
  wf.write((char *) &y, sizeof(int));
  wf.write((char *) &z, sizeof(int));

  for (int i = 0; i < x; i++) {
    for (int j = 0; j < y; j++) {
      for (int k = 0; k < z; k++) {
        coord3d *cell = &grid[i*y*z+j*z+k];
        wf.write((char *) &(cell->x), sizeof(double));
        wf.write((char *) &(cell->y), sizeof(double));
        wf.write((char *) &(cell->z), sizeof(double));
      }
    }
  }

  wf.close();
  if(!wf.good()){
    cout << "error while writing\n";
    return 1;
  }

  return 0;

}

int main(int argc, char **argv) //inputs follow format: "radius, height" with both dimensions in thou
{

  double id = str2int(argv[1])/1000.0d;
  double ir = id/2.0d;
  double od = str2int(argv[2])/1000.0d;
  double or_ = od/2.0d;
  double height = str2int(argv[3])/1000.0d;
  int cpoints = str2int(argv[4]);//number of points in the circle
  int rpoints = str2int(argv[5]);
  int hpoints = str2int(argv[6]);//number of points vertically

  cout << "\nid: " << id << "\nod: " << od << "\nh: " << height
    << "\ncpoints: " << cpoints << "\nhpoints: " << hpoints
    << "\nrpoints: " << rpoints << "\n";

  coord3d cylinder_grid[hpoints][rpoints][cpoints];
  double dtheta = 2.0d * PI / cpoints;
  double dr = (or_-ir)/rpoints;
  double startr = ir + dr/2;
  double dh = height/hpoints;
  for (int i = 0; i < hpoints; i++) {
    double h = dh/2 + dh*i;
    for (int j = 0; j < rpoints; j++) {
      double r = startr + dr*j;
      for (int k = 0; k < cpoints; k++) {
        double theta = k*dtheta;
        cylinder_grid[i][j][k].x = r * cos(theta);
        //cout << cylinder_grid[i][j][k].x << "\n";
        cylinder_grid[i][j][k].y = r * sin(theta);
        cylinder_grid[i][j][k].z = h;
      }
    }
  }

  write_grid(hpoints, rpoints, cpoints, &cylinder_grid[0][0][0], 'c');

  return 0;
}
