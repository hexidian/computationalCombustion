#include <iostream>
#include <fstream>
#include <math.h>
#include <string.h>

#include "read_grid.h"

//O2 N2
#define SPC_CT 2


struct Cell {
  double x;
  double y;
  double z;

  double sp_rho[SPC_CT];//partial densities of each species
  double momentum[3];//x,y,z
  double e;//energy per unit mass
};

int main(int argc, char **argv){

  return 1;

}
